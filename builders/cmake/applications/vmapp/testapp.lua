require "crtmpserverdefaults"

function table.val_to_str ( v )
  if "string" == type( v ) then
    v = string.gsub( v, "\n", "\\n" )
    if string.match( string.gsub(v,"[^'\"]",""), '^"+$' ) then
      return "'" .. v .. "'"
    end
    return '"' .. string.gsub(v,'"', '\\"' ) .. '"'
  else
    return "table" == type( v ) and table.tostring( v ) or
      tostring( v )
  end
end

function table.key_to_str ( k )
  if "string" == type( k ) and string.match( k, "^[_%a][_%a%d]*$" ) then
    return k
  else
    return "[" .. table.val_to_str( k ) .. "]"
  end
end

function table.tostring( tbl )
  local result, done = {}, {}
  for k, v in ipairs( tbl ) do
    table.insert( result, table.val_to_str( v ) )
    done[ k ] = true
  end
  for k, v in pairs( tbl ) do
    if not done[ k ] then
      table.insert( result,
        table.key_to_str( k ) .. "=" .. table.val_to_str( v ) )
    end
  end
  return "{" .. table.concat( result, "," ) .. "}"
end

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

customhandlers.application.initCompleted=function()
	table_print(_G);
	crtmpserver.handlers.application.pullExternalStream({uri="rtmp://edge01.fms.dutchview.nl/botr/bunny",localStreamName="gigi"});
	--crtmpserver.handlers.rtmp.generateMetaFiles();
	--print(table.tostring(crtmpserver.handlers.rtmp.getMetaData("mp4:lg.mp4")));
	--print(table.tostring(crtmpserver.handlers.rtmp.getMetaData("mp4:test1/lg.mp4")));
	return true;
end

customhandlers.rtmp.pullExternalStream=function(uri,streamConfig)
	print(table.tostring(uri));
	print(table.tostring(streamConfig));
	return crtmpserver.handlers.rtmp.pullExternalStream(uri,streamConfig);
end

--customhandlers.rtmp.processInvokePlay = function(protocolId,request)
--	if not crtmpserver.application.pullExternalStream({uri="rtmp://edge01.fms.dutchview.nl/botr/bunny",localStreamName="gigi"}) then
--		return false;
--	end
--	return crtmpserver.handlers.rtmp.processInvokePublish(protocolId,request);
--end

