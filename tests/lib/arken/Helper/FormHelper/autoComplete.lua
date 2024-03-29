local FormHelper = require('arken.Helper.FormHelper')
local test = {}

test.should_return_html = function()
  request = {}
  request.params = function()
    return {}
  end
  local controller = {controller_name = 'form', action_name = 'save'}
  local form   = FormHelper.new{ name = 'form', controller = controller, data = data }
  local result = form:autoComplete('id', {controller = 'users', action = 'list'} )
  local html   = [[<input type="text" id="id" name="id" style="width:250px">  <script>
   jQuery(function() {
     jQuery( "#id" ).autocomplete({
       source: function(request, response) {
         jQuery.ajax({
           url: '/users/list',
           dataType: 'jsonp',
           jsonp: 'json_callback',
           data: {
             featureClass: 'P',
             style: 'full',
             limit: 10,
             busca: request.term
         },
         success: function( data ){
           response( jQuery.map( data, function( item ) {
             return {
               label: item.label,
               value: item.value,
               attributes: item
             }
           }));
         },
         error: function(params1, params2, params3) {
         //alert(params1+' - '+params2+' - '+params3);
         }
       })
     },
     select: function(event, ui) { ; },
     minLength: 1
   });
  });
  </script>]]
  assert( result == html, result )
end

return test
