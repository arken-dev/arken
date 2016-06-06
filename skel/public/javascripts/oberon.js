var flag = false;
jQuery(function (jQuery) {
    var csrf_token = jQuery('meta[name=csrf-token]').attr('content');
    var csrf_param = jQuery('meta[name=csrf-param]').attr('content');

    jQuery.fn.extend({
        /**
         * Triggers a custom event on an element and returns the event result
         * this is used to get around not being able to ensure callbacks are placed
         * at the end of the chain.
         *
         * TODO: deprecate with jQuery 1.4.2 release, in favor of subscribing to our
         *       own events and placing ourselves at the end of the chain.
         */
        triggerAndReturn: function (name, data) {
            var event = new jQuery.Event(name);
            this.trigger(event, data);

            return event.result !== false;
        },

        /**
         * Handles execution of remote calls firing overridable events along the way
         */
        callRemote: function () {
            var el      = this;
            var data    = el.is('form') ? el.serializeArray() : eval('(' + el.attr('data-with') + ')') ;
            var method  = el.attr('method') || el.attr('data-method') || 'GET';
            var update  = el.attr('data-update');
            var url     = el.attr('action') || el.attr('href');
            /*
            if (el.attr('data-confirm')) {
               if (!confirm(el.attr('data-confirm'))) {
                   return false;
               }
            }
            */

            // TODO: should let the developer know no url was found
            if (url !== undefined) {
                if (el.triggerAndReturn('ajax:before')) {
                    jQuery.ajax({
                        url: url,
                        data: data,
                        type: method.toUpperCase(),
                        beforeSend: function (xhr) {
                            //jQuery("#modalTransaction").show();
                            xhr.setRequestHeader("Accept", "text/javascript");
                            el.trigger('ajax:loading', xhr);
                        },
                        success: function (data, status, xhr) {
                            el.trigger('ajax:success', [data, status, xhr]);
                            //alert(xhr.responseText); //debug
                            if(xhr.getResponseHeader('Content-Type').match(/text\/html/) ) {
			        if(update == undefined) {
                                  jQuery('#content').html(xhr.responseText);
				} else {
                                  jQuery('#' + update).html(xhr.responseText);
				}
                            }
                        },
                        complete: function (xhr) {
                            el.trigger('ajax:complete', xhr);
                            if(objContext != null) {
                              objContext.style.backgroundColor ='#f4f5f7';
                            }
                            //jQuery("#modalTransaction").hide();
                        },
                        error: function (xhr, status, error) {
                            jQuery('#processar_pagamento_loading').hide();
                            jQuery('#processar_fale_conosco_loading').hide();
                            alert(xhr.responseText);
                            el.trigger('ajax:failure', [xhr, status, error]);
                        }
                    });
                }

                el.trigger('ajax:after');
            }
        }
    });


    /**
     *  condition handler
     */
    jQuery('a[data-condition],input[data-condition],form[data-condition]').live('click', function () {
        var el = jQuery(this);
        if(! eval(el.attr('data-condition'))) {
          return false;
        }
    });

    /**
     *  confirmation handler
     */
    jQuery('a[data-confirm],input[data-confirm],form[data-confirm]').live('click', function () {
        var el = jQuery(this);
        if(! window.confirm(el.attr('data-confirm'))) {
          return false;
        }
    });


    /**
     * remote handlers
     */
    jQuery('form[data-remote="true"]').live('submit', {flag: flag}, function (e) {
        //if(! e.data.flag) {
            e.data.flag = true
            jQuery(this).callRemote();
            e.preventDefault();
            //alert('submit');
        //}
    });

    jQuery('a[data-remote="true"],input[data-remote="true"]').live('click', function (e) {
        /* condition */
        var el = jQuery(this);
        if(el.attr('data-condition')) {
          if(! eval(el.attr('data-condition'))) {
            return false;
          }
        }
        /* condition */
        jQuery(this).callRemote();
        e.preventDefault();
    });

    jQuery('a[data-method][data-remote!=true]').live('click',function(e){
        var link = jQuery(this);
        var href = link.attr('href');
        var method = link.attr('data-method');
        var form = jQuery('<form method="post" action="'+href+'">');
        var input = jQuery('<input name="_method" value="'+method+'" type="hidden" />');
        var csrf_input = jQuery('<input name="'+csrf_param+'" value="'+csrf_token+'" type="hidden" />');

        form.hide()
            .append(input)
            .append(csrf_input)
            .appendTo('body'); // redundant?

        e.preventDefault();
        form.submit();
    });

    /**
     * disable_with handlers
     */
    jQuery('form[data-remote="true"]').live('ajax:before', function () {
        jQuery(this).children('input[data-disable-with]').each(function () {
            var input = jQuery(this);
            input.data('enable_with', input.val())
                 .attr('value', input.attr('data-disable-with'))
                 .attr('disabled', 'disabled');
        });
    });

    jQuery('form[data-remote="true"]').live('ajax:after', function () {
        jQuery(this).children('input[data-disable-with]').each(function () {
            var input = jQuery(this);
            input.removeAttr('disabled')
                 .val(input.data('enable_with'));
        });
    });
});
