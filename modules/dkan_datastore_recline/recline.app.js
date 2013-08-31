/**
 * @file
 * Provides options for recline visualization.
 */

//
// if api exists, use that
// elseif dataproxy available use that
// else use csv
//
// - allow control of desired backend
// - return better response if it times out
// - use features override to control behavior of recline as field formatter
//
//
// 1) create new field formater for file upload or link
// 2) if link to file use dataproxy
// 3) field formatter checks if api exists, uses dataproxy if available, else
// uses csv, keeps setting in field formatter settings
// 4) 
//
//
//
//
// Data API link
// - fix datastore_search to actually search
// - show message on data api link if file exists but no resource has been uploaded
// - fix install profile so files can be added to the datastore
// - fix exeption on API if file exists but has not been added to the datastore
//
//
(function ($) {
  Drupal.behaviors.Recline = {
    attach: function (context) {
      file = Drupal.settings.recline.file;
      grid = Drupal.settings.recline.grid;
      graph = Drupal.settings.recline.graph;
      map = Drupal.settings.recline.map;
      uuid = Drupal.settings.recline.uuid;

      window.dataExplorer = null;
      window.explorerDiv = $('.data-explorer');

      var state = recline.View.parseQueryString(decodeURIComponent(window.location.hash));
      if ('#map' in state) {
        state['currentView'] = 'map';
      } else if ('#graph' in state) {
        state['currentView'] = 'graph';
      } else if ('#timeline' in state) {
        state['currentView'] = 'timeline';
      }
      var DKAN_API = '/api/action/datastore/search.json';
      var url = window.location.origin + DKAN_API + '?resource_id=' + uuid;
      var DkanDatastore = false;
      var DkanApi = $.ajax({
        type: 'GET',
        url: url,
        dataType: 'json',
        success: function(data, status) {
          if ('success' in data && data.success) {
            DkanDatastore = true;
          }
        },
      });
      DkanApi.done(function(data) {
        if (DkanDatastore) {
          var dataset = new recline.Model.Dataset({
            uuid: uuid,
            url: url,
            backend: 'dkan',
          });
        } else if (fileType = 'text/csv') {
          var dataset = new recline.Model.Dataset({
             url: file,
             backend: 'csv',
          });
        // TODO: check filetype if xls.
        }
        dataset.fetch();
        createExplorer(dataset, state);
      });
    }
  }

  // make Explorer creation / initialization in a function so we can call it
  // again and again
  var createExplorer = function(dataset, state) {
    // remove existing data explorer view
    var reload = false;
    if (window.dataExplorer) {
      window.dataExplorer.remove();
      reload = true;
    }
    window.dataExplorer = null;
    var $el = $('<div />');
    $el.appendTo(window.explorerDiv);

    var views = [];
    if (grid) {
      views.push(
        {
          id: 'grid',
          label: 'Grid',
          view: new recline.View.SlickGrid({
            model: dataset
          }),
        }
      );
    }
    if (graph) {
      views.push(
      {
        id: 'graph',
        label: 'Graph',
        view: new recline.View.Graph({
          model: dataset
        }),
      }
      );
    }
    if (map) {
      views.push(
      {
        id: 'map',
        label: 'Map',
        view: new recline.View.Map({
          model: dataset
        }),
      }
      );
    }

    window.dataExplorer = new recline.View.MultiView({
      model: dataset,
      el: $el,
      state: state,
      views: views
    });
  }

})(jQuery);
