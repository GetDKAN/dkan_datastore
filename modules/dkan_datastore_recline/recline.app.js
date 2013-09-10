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
      dkan = Drupal.settings.recline.dkan;
      fileType = Drupal.settings.recline.fileType;

      window.dataExplorer = null;
      window.explorerDiv = $('.data-explorer');

      // This is the very basic state collection.
      var state = recline.View.parseQueryString(decodeURIComponent(window.location.hash));
      if ('#map' in state) {
        state['currentView'] = 'map';
      } else if ('#graph' in state) {
        state['currentView'] = 'graph';
      } else if ('#timeline' in state) {
        state['currentView'] = 'timeline';
      }
      if (dkan) {
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
          error: function(data, status) {
            $('.data-explorer').append('<div class="messages status">Unable to connect to the datastore.</div>');
          },
        });
        DkanApi.done(function(data) {
          if (DkanDatastore) {
            var dataset = new recline.Model.Dataset({
              endpoint: window.location.origin + '/api',
              url: url,
              id: uuid,
              backend: 'ckan',
            });
            dataset.fetch();
            return createExplorer(dataset, state);
          }
          else {
          }
        });
      }
      if (fileType == 'text/csv') {
        $.ajax({
          url: file,
          timeout: 1000,
          success: function(data) {
            var dataset = new recline.Model.Dataset({
               data: data,
               backend: 'csv',
            });
            dataset.fetch();
            createExplorer(dataset, state);
          },
          error: function(x, t, m) {
            if (t === "timeout") {
              $('.data-explorer').append('<div class="messages status">File was too large or unavailable for preview.</div>');
            } else {
              $('.data-explorer').append('<div class="messages status">Data preview unavailable.</div>');
            }
          }
        });
      // TODO: check filetype if xls.
      }
      else if (fileType == 'application/vnd.openxmlformats-officedocument.spreadsheetml.sheet' || fileType == 'application/vnd.ms-excel') {
        var dataset = new recline.Model.Dataset({
          url: file,
          backend: 'dataproxy',
        });
        dataset.fetch();
        if (dataset.recordCount) {
          createExplorer(dataset, state);
        }
        else {
          $('.data-explorer').append('<div class="messages status">Unable to retreive data for selected file. This may be do to an interuption with the DataProxy service.</div>');
        }
      }
      else {
        $('.data-explorer').append('<div class="messages status">File type ' + fileType + ' not supported for preview.</div>');
      }
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
