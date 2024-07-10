/* Copyright 2015-present Samsung Electronics Co., Ltd. and other contributors
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

function isString(arg) {
  return typeof arg === 'string';
}

function formatValue(v) {
  if (v === undefined) {
    return 'undefined';
  } else if (v === null) {
    return 'null';
  } else if (Array.isArray(v)) {
    return '[' + v.toString() + ']';
  } else if (v instanceof Error) {
    return v.toString();
  } else if (typeof v === 'object') {
    return JSON.stringify(v, null, 2);
  } else {
    return v.toString();
  }
}





function format(s) {

  var i;

  if (!isString(s)) {

    var arrs = [];

    for (i = 0; i < arguments.length; ++i) {

      arrs.push(formatValue(arguments[i]));

    }

    return arrs.join(' ');

  }

 

  i = 1;

  var args = arguments;

  var arg_string;

  var str = '';

  var start = 0;

  var end = 0;

  while (end < s.length) {

    if (s.charAt(end) !== '%') {

      end++;

      continue;

    }

 

    str += s.slice(start, end);

 

    switch (s.charAt(end + 1)) {

      case 's':

        arg_string = String(args[i]);

        break;

      case 'd':

        arg_string = Number(args[i]);

        break;

      case 'j':

        try {

          arg_string = JSON.stringify(args[i]);

        } catch (_) {

          arg_string = '[Circular]';

        }

        break;

      case '%':

        str += '%';

        start = end = end + 2;

        continue;

      default:

        str = str + '%' + s.charAt(end + 1);

        start = end = end + 2;

        continue;

    }

 

    if (i >= args.length) {

      str = str + '%' + s.charAt(end + 1);

    } else {

      i++;

      str += arg_string;

    }

 

    start = end = end + 2;

  }

 

  str += s.slice(start, end);

 

  while (i < args.length) {

    str += ' ' + formatValue(args[i++]);

  }

 

  return str;

}









js_console_log('console.js ok' );


function Console() {
}

Console.prototype.log =Console.prototype.info = function() {
    js_console_log('Info: '+format.apply(this,  arguments) );
}

Console.prototype.warn = function() {
  js_console_log('Warn: '+format.apply(this,  arguments) );
}

Console.prototype.error = function() {
  js_console_log('Error: '+format.apply(this,  arguments) );
}


console = new Console();


