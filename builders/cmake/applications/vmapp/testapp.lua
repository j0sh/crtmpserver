require "crtmpserverdefaults"

-- Print anything - including nested tables
function table_print (tt, indent, done)
  done = done or {}
  indent = indent or 0
  if type(tt) == "table" then
    for key, value in pairs (tt) do
      io.write(string.rep (" ", indent)) -- indent it
      if type (value) == "table" and not done [value] then
        done [value] = true
        io.write(string.format("[%s] => table\n", tostring (key)));
        io.write(string.rep (" ", indent+4)) -- indent it
        io.write("(\n");
        table_print (value, indent + 7, done)
        io.write(string.rep (" ", indent+4)) -- indent it
        io.write(")\n");
      else
        io.write(string.format("[%s] => %s\n",
            tostring (key), tostring(value)))
      end
    end
  else
    io.write(tt .. "\n")
  end
end

customhandlers.rtmp.processInvokeGeneric = function(protocolId,request)
	print (request.invoke.functionName.." invoked. Parameters");
	table_print(request.invoke.parameters);
	if request.invoke.functionName == "ClientServerWithResponse" then
		return crtmpserver.handlers.rtmp.sendResponse(protocolId,request,{"some complex object",subObject={doubleVal=1.2, str="string value",true,123.45}});
	elseif request.invoke.functionName == "ClientServerWithoutResponse" then
		return true;
	elseif request.invoke.functionName == "TriggerServerClientWithResponse" then
		return crtmpserver.handlers.rtmp.sendRequest(protocolId,"ServerClientWithResponse",true,{"some complex object",subObject={doubleVal=1.2, str="string value",true,123.45}},true,123.456,"someString");
	elseif request.invoke.functionName == "TriggerServerClientWithoutResponse" then
        return crtmpserver.handlers.rtmp.sendRequest(protocolId,"ServerClientWithoutResponse",false,{"some complex object",subObject={doubleVal=1.2, str="string value",true,123.45}},true,123.456,"someString");
	else
		return crtmpserver.handlers.rtmp.processInvokeGeneric(protocolId,request)
	end
end

customhandlers.rtmp.processInvokeGenericResult = function(protocolId,request,response)
	print("We have a response from one of our previous requests");
	table_print({request=request,response=response});
	return false;
end

