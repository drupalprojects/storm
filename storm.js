function storm_empty_select(_select) {
  if(_select == undefined) return;
  for (var i=_select.length-1; i>=0; i--) {
    _select.remove(i);
  }
};

function storm_fill_select(_select, _items, _with_all_option, _all_text) {
  if(_select == undefined) return;

  if(_with_all_option) {
    opt=new Option();
    opt.value = 0;
    opt.text = _all_text;
    try
    {
      _select.add(opt, null); // standards compliant
    }
    catch(ex)
    {
      _select.add(opt); // IE only
    }
  }

  for (key in _items) {
    opt=new Option();
    opt.value = key;
    opt.text = _items[key];
    try
    {
      _select.add(opt, null); // standards compliant
    }
    catch(ex)
    {
      _select.add(opt); // IE only
    }
  }
};

function storm_popup(sender, name, title, width, height, content_id, position) {
  var p_name = "storm" + name + '_popup';
  var p_close = "storm" + name + "_popup_close";

  var elemOffset = storm_offset($(sender));
  var top = elemOffset.top;
  var left = elemOffset.left;
alert(left);
  switch (position) {
    case 'l':
      left = left-width;
    break;
    case 'lt':
      left = left - width;
      top = top - height;   
    break;
    case 't':
      left = left - Math.floor(width / 2); 
      top = top - height; 
    break;
    case 'rt':
      top = top - height; 
    break;
    case 'r':
    break;
    case 'rb':
    break;
    case 'b':
      left = left - Math.floor(width / 2); 
    break;
    case 'lb':
      left = left-width;
    break;
  }

  $("#" + p_name).remove();
  var popup = '<div class="storm_popup" id="' + p_name + '">';
  popup += '<div class="storm_popup_title" id="' + p_close + '">' + title + '</div>';
  popup += '<div class="storm_popup_inner">';
  popup += $("#" + content_id).html();
  popup += "</div>";
  popup += "</div>";
  $("body").append(popup);
  var p = $("#" + p_name);
  p.css('position', 'absolute');
  p.css('top', top);
  p.css('left', left);
  p.css('width', width);
  p.css('height', height);
  p.show();
  $("#" + p_close).click(function(){
    $("#" + p_name).remove();
    return false;
  });
};

function storm_datext_tonull(sender, date_id) {
  if (sender.value == "-1") {
    $("#" + date_id + '-day').val("-1");
    $("#" + date_id + '-month').val("-1");
    $("#" + date_id + '-year').val("-1");
  }
};

function storm_offset(elem) {
	var left = 0, top = 0, results;
	if ( elem ) with ( jQuery.browser ) {
		var parent       = elem.parentNode, 
		    offsetChild  = elem,
		    offsetParent = elem.offsetParent, 
		    doc          = elem.get(0).ownerDocument,
		    safari2      = safari && parseInt(version) < 522 && !/adobeair/i.test(userAgent),
		    fixed        = elem.css("position") == "fixed";
		// Use getBoundingClientRect if available

		if ( elem.getBoundingClientRect ) {
			var box = elem.getBoundingClientRect();
		
			// Add the document scroll offsets
			add(box.left + Math.max(doc.documentElement.scrollLeft, doc.body.scrollLeft),
				box.top  + Math.max(doc.documentElement.scrollTop,  doc.body.scrollTop));
		
			// IE adds the HTML element's border, by default it is medium which is 2px
			// IE 6 and 7 quirks mode the border width is overwritable by the following css html { border: 0; }
			// IE 7 standards mode, the border is always 2px
			// This border/offset is typically represented by the clientLeft and clientTop properties
			// However, in IE6 and 7 quirks mode the clientLeft and clientTop properties are not updated when overwriting it via CSS
			// Therefore this method will be off by 2px in IE while in quirksmode
			add( -doc.documentElement.clientLeft, -doc.documentElement.clientTop );
	
		// Otherwise loop through the offsetParents and parentNodes
		} else {
		
			// Initial element offsets
			add( elem.offsetLeft, elem.offsetTop );
			
			// Get parent offsets
			while ( offsetParent ) {
				// Add offsetParent offsets
				add( offsetParent.offsetLeft, offsetParent.offsetTop );
			
				// Mozilla and Safari > 2 does not include the border on offset parents
				// However Mozilla adds the border for table or table cells
				if ( mozilla && !/^t(able|d|h)$/i.test(offsetParent.tagName) || safari && !safari2 )
					border( offsetParent );
					
				// Add the document scroll offsets if position is fixed on any offsetParent
				if ( !fixed && offsetParent.css("position") == "fixed" )
					fixed = true;
			
				// Set offsetChild to previous offsetParent unless it is the body element
				offsetChild  = /^body$/i.test(offsetParent.tagName) ? offsetChild : offsetParent;
				// Get next offsetParent
				offsetParent = offsetParent.offsetParent;
			}
		
			// Get parent scroll offsets
			while ( parent && parent.tagName && !/^body|html$/i.test(parent.tagName) ) {
				// Remove parent scroll UNLESS that parent is inline or a table to work around Opera inline/table scrollLeft/Top bug
				if ( !/^inline|table.*$/i.test(parent.css("display")) )
					// Subtract parent scroll offsets
					add( -parent.scrollLeft, -parent.scrollTop );
			
				// Mozilla does not add the border for a parent that has overflow != visible
				if ( mozilla && parent.css("overflow") != "visible" )
					border( parent );
			
				// Get next parent
				parent = parent.parentNode;
			}
		
			// Safari <= 2 doubles body offsets with a fixed position element/offsetParent or absolutely positioned offsetChild
			// Mozilla doubles body offsets with a non-absolutely positioned offsetChild
			if ( (safari2 && (fixed || offsetChild.css("position") == "absolute")) || 
				(mozilla && offsetChild.css("position") != "absolute") )
					add( -doc.body.offsetLeft, -doc.body.offsetTop );
			
			// Add the document scroll offsets if position is fixed
			if ( fixed )
				add(Math.max(doc.documentElement.scrollLeft, doc.body.scrollLeft),
					Math.max(doc.documentElement.scrollTop,  doc.body.scrollTop));
		}

		// Return an object with top and left properties
		results = { top: top, left: left };
	}

	function border(elem) {
		add( elem.curCSS("borderLeftWidth", true), elem.curCSS("borderTopWidth", true) );
	}

	function add(l, t) {
		left += parseInt(l) || 0;
		top += parseInt(t) || 0;
	}

	return results;
};
