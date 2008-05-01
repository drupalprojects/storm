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

