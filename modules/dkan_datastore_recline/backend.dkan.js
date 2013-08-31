
this.recline = this.recline || {};
this.recline.Backend = this.recline.Backend || {};
this.recline.Backend.Dkan = this.recline.Backend.Dkan || {};

(function(my) {

  my.__type__ = 'dkan';

  var Deferred = jQuery.Deferred;

  // Default CKAN API endpoint used for requests (you can change this but it will affect every request!)
  //
  // DEPRECATION: this will be removed in v0.7. Please set endpoint attribute on dataset instead
  my.API_ENDPOINT = 'http://dkan.local/api';

  // ### fetch
  my.fetch = function(dataset) {
    console.log('ssf');
    var wrapper;
    if (dataset.endpoint) {
      wrapper = my.DataStore(dataset.endpoint);
    } else {
      var out = my._parseCkanResourceUrl(dataset.url);
      dataset.id = out.resource_id;
      wrapper = my.DataStore(out.endpoint);
    }
    var dfd = new Deferred();
    var jqxhr = wrapper.search({resource_id: dataset.id, limit: 0});
    jqxhr.done(function(results) {
      // map dkan types to our usual types ...
      var fields = _.map(results.result.fields, function(field) {
        field.type = field.type in DKAN_TYPES_MAP ? DKAN_TYPES_MAP[field.type] : field.type;
        return field;
      });
      var out = {
        fields: fields,
        useMemoryStore: false
      };
      dfd.resolve(out);
    });
    return dfd.promise();
  };

  // only put in the module namespace so we can access for tests!
  my._normalizeQuery = function(queryObj, dataset) {
    var actualQuery = {
      resource_id: dataset.id,
      q: queryObj.q,
      filters: {},
      limit: queryObj.size || 10,
      offset: queryObj.from || 0
    };

    if (queryObj.sort && queryObj.sort.length > 0) {
      var _tmp = _.map(queryObj.sort, function(sortObj) {
        return sortObj.field + ' ' + (sortObj.order || '');
      });
      actualQuery.sort = _tmp.join(',');
    }

    if (queryObj.filters && queryObj.filters.length > 0) {
      _.each(queryObj.filters, function(filter) {
        if (filter.type === "term") {
          actualQuery.filters[filter.field] = filter.term;
        }
      });
    }
    return actualQuery;
  };

  my.query = function(queryObj, dataset) {
    var wrapper;
    if (dataset.endpoint) {
      wrapper = my.DataStore(dataset.endpoint);
    } else {
      var out = my._parseCkanResourceUrl(dataset.url);
      dataset.id = out.resource_id;
      wrapper = my.DataStore(out.endpoint);
    }
    var actualQuery = my._normalizeQuery(queryObj, dataset);
    var dfd = new Deferred();
    var jqxhr = wrapper.search(actualQuery);
    jqxhr.done(function(results) {
      var out = {
        total: results.result.total,
        hits: results.result.records
      };
      dfd.resolve(out);
    });
    return dfd.promise();
  };

  // ### DataStore
  //
  // Simple wrapper around the DKAN DataStore API
  //
  // @param endpoint: CKAN api endpoint (e.g. http://datahub.io/api)
  my.DataStore = function(endpoint) {
    var that = {endpoint: endpoint || my.API_ENDPOINT};

    that.search = function(data) {
      var searchUrl = that.endpoint + '/action/datastore/search.json';
      var jqxhr = jQuery.ajax({
        url: searchUrl,
        type: 'POST',
        data: JSON.stringify(data)
      });
      return jqxhr;
    };

    return that;
  };

  // Parse a normal CKAN resource URL and return API endpoint etc
  //
  // Normal URL is something like http://demo.dkan.org/dataset/some-dataset/resource/eb23e809-ccbb-4ad1-820a-19586fc4bebd
  my._parseCkanResourceUrl = function(url) {
    parts = url.split('/');
    var len = parts.length;
    return {
      resource_id: parts[len-1],
      endpoint: parts.slice(0,[len-4]).join('/') + '/api'
    };
  };

  var DKAN_TYPES_MAP = {
    'int4': 'integer',
    'int8': 'integer',
    'float8': 'float'
  };

}(this.recline.Backend.Dkan));
