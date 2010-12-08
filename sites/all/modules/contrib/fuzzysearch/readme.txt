Fuzzy Search Module Read Me Project Home: http://drupal.org/project/fuzzysearch

=== Installation === - The following installation steps assume your drupal
installation is located at http://example.com with clean urls turned on.

To install this module simply:
1. Install as usual, see http://drupal.org/node/70151 for further information.
2. You should probably use a stopwords file to keep common words from bloating
   your index. See fuzzysearch/stopwords/README.txt
3. At admin/build/block put the "Fuzzy search form into a region in your theme.
4. Visit the fuzzysearch admin settings page to see how much of your site has
   been indexed. Run cron until it is 100% indexed.
5. Set up a regular cron job to keep your site fully indexed.

=== What is indexed? ===

Currently this module indexes all filtered node content, taxonomy terms
associated with the nodes, cck text fields associated with a node, comments left
on the node, and any text being returned by the call to hook_nodeapi with the
$op = 'update index'.

=== Fuzzy Search Settings ===

*Checking the display scoring checkbox is helpful for debugging when you are
trying to fine tune score modifiers.  It will output completeness and score
values under each of the returned results.

*Setting a minimum completeness is helpful for ensuring that words are returned
only if a high enough portion of a search term matches. It is best to set this
value 10 points below your ideal minimum percentage. So if you wanted to match
results with at least 50% of the word matching, set this value to 40.  The
match is calculated per indexed word and not by the search phrase, ensuring that
matches are relevant to the words in the phrase and not just all the letter
combinations in the phrase.

*Also note that when a phrase matches more than a single word the completeness
can be higher than 100%, this is because the completeness of each word is summed
and then sorted as a measure of accuracy in the result set.

* As of Drupal 6 you can filter results output by node type. This does not
affect search indexing, so you don't have to reindex if you change this.

* As of Drupal 6 you can choose the ngram length. This is the size of the chunks
words are broken into on indexing and searching. The default value is 3. The
lower the value, the more results (and more noise) you will get. Also, a lower
value will increase the size of your fuzzysearch_index table.

* As of Drupal 6 Fuzzy Search will try to highlight misspelled words. You can
set the accuracy by choosing a minimum spelling score from 0 to 100, where 100
means no misspellings are highlighted. You do not need to reindex when you
change this setting.

This provides a fuzzy spelling check by replacing bad (misspelled) ngrams with
a wildcard. It is possible to get false matches. For example, searching for
"rendition" will also highlight "condition" if your spelling score is low
enough. However, these kinds of matches are likely to have lower score
completeness and be sorted to the bottom of your results if your search term
exists in your content.

=== Fuzzy Search Blocks ===

Fuzzy search form: This block provides the form where users will entier the
search terms.

Fuzzy search title query: The Drupal 6 version provides a block that performs a
fuzzysearch on a query in the path. This may be performance intensive, so use
with caution. If the fuzzysearch query is in the path, the block will return
search matches of node titles. It's up to you to put the query in your path like
this:

http://example/node/add/question?fuzzysearch=arthritis%20knees%20pain

This would be good for similar content blocks, or to suggest existing content
before letting the user create new content.

=== Theming ===

As of Drupal 6, the module provides a template, fuzzysearch-result.tpl.php, that
you can copy to your theme folder and modify. This affects the search results
page. There are some theme functions you can override to theme the fuzzysearch
block, and you can also override block.tpl.php.

=== About Fuzzy Search ===

This module provides a fuzzy matching search engine for nodes.
Nodes are indexed when the site's cron job is run.  The module automatically
queues a node for indexing once it is submitted/updated or a comment has been
made on it.  Nodes can also be queued for reindexing by other modules when the
function fuzzysearch_reindex($nid, $module) is called,  Where $nid is the nid of
the node to have reindexed and $module is a string containing an identifier of
the module calling for the node to be reindexed.

Fuzzy matching is implemented by using qgrams.  Each word in a node is split
into 3 (default) letter lengths, so 'apple' gets indexed with 3 smaller strings 'app',
'ppl', 'ple'.  The effect of this is that as long as your search matches X
percentage (administerable in the admin settings) of the word the node will be
pulled up in the results.  One issue that is inherent with this method is cases
when a user searches for a word like 'athens' which contains the word 'the'
within it and has a completeness of 100%. In order to account for this
larger length words qgrams must match qgrams from words with a similar length.
This is an imperfect solution but it does a good job of returning the most
relevant results.

=== Fuzzysearch Submodules ===

Fuzzysearch comes with the following example submodules:

1. fuzzysearch_filter_example
   When enabled, this module provides an example of how to use
   hook_fuzzysearch_filter().

   See the API section below for information about this hook.

=== Fuzzysearch API ===

=== hook_fuzzysearch_score($op, $node) ===

This hook allows other contributed modules to modify the score of any
node being indexed. This affects nodes, not words. Site administrators can then
set how important these modifiers are to their particular site's use.  Changing
the modifier score to 0 means that the modification being returned by that
particular module will have no effect on the scoring of the nodes on the site.
Setting the modifier to 10 means it will have maximum effect.

This simple example code from a contributed module implementing the scoring hook
returns a score of 5 if the node author is user 1. Any time a node is changed
fuzzysearch will apply the modifiers on the next cron run. You must reindex your
site to affect existing nodes, or resave the nodes.

/**
 * Implementation of hook_fuzzysearch_score
 * @param $op 'settings' returns array with information about the module (seen
 *  in the admin settings form) 'index' returns a score modifier to the node
 *  being indexed.
*/

function custom_fuzzysearch_score($op, $node) {
  switch ($op) {
    case 'settings':
      $info[] = array(
        'id' => 'user_1',
        'title' => t('Author is User 1'),
        'description' => t('This multiplier lets you increase the score of nodes authored by user 1.'),
      );
      return $info;
      break;
    case 'index':
      $score = $node->uid == 1 ? 5 : 0;

      $scores[] = array(
        'id' => 'user_1',
        'score' => $score,
      );
      return $scores;
  }
}

=== hook_fuzzysearch_index($node) ===
Before fuzzysearch indexes a node, other modules have the chance to change the
node or prevent it from being indexed. If a module implements this hook and
returns FALSE the node will not be indexed. If it already existed in the index
it will be removed. Modules should check that they have a node object to work
with, as another module may have already returned FALSE.

Changes returned to the node object are only reflected in the fuzzysearch index,
not in the node as saved in the database.

Some uses for this hook include preventing a node type from being indexed,
boosting a node's search score for certain words, or adding additional text to
a node. This is slightly different than hook_nodeapi's update index operation in
that you can replace or change parts of the node rather than just adding text.

Example code of a contributed module implementing the indexing hook:

// Prevent private nodes from being indexed by fuzzy search.
function custom_fuzzysearch_index($node) {
  if (!is_object($node) || $node->type == 'private') {
    return FALSE;
  }
  else {
    return $node;
  }
}

=== hook_fuzzysearch_filter($op, $text) ===

Hook_fuzzysearch_filter($text) gives modules an opportunity to filter the the text
to be indexed before it is indexed and/or searched. The common use for this is
to do more complicated filtering than is allowed by the stop words text files.

$op == 'index' will filter words on indexing of content. $op == 'search' will 
filter the search terms before the index is searched for results.

=== About The Author ===

This module was created for Drupal as part of Google Summer of Code 2007.

Blake Lucchesi www.boldsource.com blake@boldsource.com

Drupal 6 version maintained by awolfey.