<?php

/**
 * @file
 * Handler to allow joins between records via a linking table
 *
 * Example Usage:
 *   To join the analysis table to the feature table through the analysisfeature table,
 *   (ie: get analysis fields to show up in a feature view)
 *   Use the following code in the analysis hook_views_data:
 *   @code
        $data['analysis']['table']['join']['feature'] = array(
          'linking' => array(
            'table' => 'analysisfeature',
            'left_field' => 'feature_id',
            'field' => 'analysis_id',
          ),
          'left_field' => 'feature_id',
          'field' => 'analysis_id',
          'handler' => 'views_handler_join_chado_through_linking'
        );
 *   @endcode
 *
 * NOTE: If the right table is in the drupal schema rather then the chado schema
 *  (ie: node, chado_feature) then add the following to the above join description:
 *  @code
    'table_is_drupal' => TRUE
 *  @endcode
 *  This will ensure the drupal table is surrounded by { } and as such any database
 *  prefixes are added correctly. If the left table is in the drupal schema it should already
 *  be defined by a previous join (or the From clause).
 */
class views_handler_join_chado_through_linking extends views_handler_join_chado_aggregator {

  // PHP 4 doesn't call constructors of the base class automatically from a
  // constructor of a derived class. It is your responsibility to propagate
  // the call to constructors upstream where appropriate.
  function construct($table = NULL, $left_table = NULL, $left_field = NULL, $field = NULL, $extra = array(), $type = 'LEFT', $added = NULL) {
    parent::construct($table, $left_table, $left_field, $field, $extra, $type);
    //dpm('in constructor');
  }

  /**
   * Creates SQL for both joins table => linking_table and linking_table => left_table
   * NOTE: Uses fields in definition as passed in from hook_views_data join definition
   */
  function join($table, &$query) {
    $output = array();

    // Table => Linking
    $opt = array(
      'table' => $this->definition['table'],
      'field' => $this->definition['field'],
      'left_table' => $this->definition['linking']['table'],
      'left_field' => $this->definition['linking']['field'],
      'table_aggregated' => $this->definition['table_aggregated'],
      'sort' => $this->sort,
      'filter' => $this->filter,
      'postgresql_9up' => $this->postgresql_9up,
    );
    //dpm($opt, 'opt for table => linking in join handler');
    $output = array_merge($output, $this->aggregate_join($query, $opt));

    // Linking => Left Table
    $opt = array(
      'table' => $this->definition['linking']['table'],
      'field' => $this->definition['linking']['left_field'],
      'left_table' => $this->definition['left_table'],
      'left_field' => $this->definition['left_field'],
      'table_aggregated' => $this->definition['table_aggregated'],
      'sort' => $this->sort,
      'filter' => $this->filter,
      'postgresql_9up' => $this->postgresql_9up,
    );
    $output = array_merge($output, $this->aggregate_join($query, $opt));

    //dpm($output, 'output in linking join');
    return implode("\n", $output);
  }

}
