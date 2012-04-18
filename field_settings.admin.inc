<?php

/**
 * Menu callback; lists all defined fields for quick reference.
 */
function field_settings_fields_list() {
  $instances = field_info_instances();
  $field_types = field_info_field_types();
  $bundles = field_info_bundles();

  $modules = system_rebuild_module_data();

  $header = array(t('Field name'), t('Field type'), t('Used in'), t('Op'));
  $rows = array();
  foreach ($instances as $entity_type => $type_bundles) {
    foreach ($type_bundles as $bundle => $bundle_instances) {
      foreach ($bundle_instances as $field_name => $instance) {
        $field = field_info_field($field_name);

        // Initialize the row if we encounter the field for the first time.
        if (!isset($rows[$field_name])) {
          $rows[$field_name]['class'] = $field['locked'] ? array('menu-disabled') : array('');
          $rows[$field_name]['data'][0] = $field['locked'] ? t('@field_name (Locked)', array('@field_name' => $field_name)) : $field_name;
          $module_name = $field_types[$field['type']]['module'];
          $rows[$field_name]['data'][1] = $field_types[$field['type']]['label'] . ' ' . t('(module: !module)', array('!module' => $modules[$module_name]->info['name']));

          // Add the Operations Column
          $rows[$field_name]['data'][3][] = l(t('Details'), 'admin/reports/fields/'.$field_name);
          $rows[$field_name]['data'][3][] = l(t('Edit'), 'admin/reports/fields/'.$field_name.'/edit');
        }

        // Add the current instance.
        $admin_path = _field_ui_bundle_admin_path($entity_type, $bundle);
        $rows[$field_name]['data'][2][] = $admin_path ? l($bundles[$entity_type][$bundle]['label'], $admin_path . '/fields') : $bundles[$entity_type][$bundle]['label'];

        // Sort the fields so that they line up.
        ksort($rows[$field_name]['data']);
      }
    }
  }
  foreach ($rows as $field_name => $cell) {
    $rows[$field_name]['data'][2] = implode(', ', $cell['data'][2]);
    $rows[$field_name]['data'][3] = implode(', ', $cell['data'][3]);
  }
  if (empty($rows)) {
    $output = t('No fields have been defined yet.');
  }
  else {
    // Sort rows by field name.
    ksort($rows);
    $output = theme('table', array('header' => $header, 'rows' => $rows));
  }
  return $output;
}

/**
 * Menu callback; simply uses devel's kpr to print the field info
 */
function field_settings_field_details($field_name) {
  $field = field_info_field($field_name);

  // I'd love to use my own field info render function instead of kpr,
  // but alas, I cannot render fieldsets without FAPI.
  $output = kpr($field, TRUE);
  //$output = field_settings_render_field_info($field);

  return $output;
}