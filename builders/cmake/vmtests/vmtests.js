function odump(object, depth, max){
  depth = depth || 0;
  max = max || 2;

  if (depth > max)
    return false;

  var indent = "";
  for (var i = 0; i < depth; i++)
    indent += "  ";

  var output = "";  
  for (var key in object){
    output += "\n" + indent + key + "["+typeof object[key]+"]: ";
    switch (typeof object[key]){
      case "object": output += odump(object[key], depth + 1, max); break;
      case "function": output += "function"; break;
      default: output += object[key]; break;        
    }
  }
  return output;
}

function echoFunctionCall(param1) {
    param1["____THIS_IS_ADDED____"]="____THIS_IS_ADDED____";
	//log(odump(param1));
    return param1;
}

