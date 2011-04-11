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
	if request.invoke.functionName == "getAvailableFlvs" then
		-- get the configuration
		configuration=crtmpserver.handlers.application.getConfig();
		
		-- get the media folder path from configuration and normalize it
		mediaFolder=crtmpserver.normalizePath(configuration.mediaFolder);

		-- list that folder
		files={crtmpserver.listFolder(mediaFolder)};

		-- process each entry in files set and generate the proper flash friendly names
		local result={};
		for _,v in pairs(files) do
			flashName=getFlashName(result,mediaFolder,v);
		end

		-- call SetAvailableFlvs on the flsh side with the results
		return crtmpserver.handlers.rtmp.sendRequest(protocolId,"SetAvailableFlvs",false,result)
	else
		return crtmpserver.handlers.rtmp.processInvokeGeneric(protocolId,request)
	end
end


function getFlashName(result,mediaFolder,filePath)
	file=crtmpserver.splitFileName(filePath)
	start,length=string.find(file.name,mediaFolder,1,true);
	if start == nil or start~=1 then return end 
	if file.extension == "mp3" then 
		table.insert(result,"mp3:"..string.sub(file.name,start+length+1))
	elseif file.extension == "mp4" or file.extension == "m4a" or file.extension == "m4v" or file.extension == "mov" or file.extension == "f4v" then
		table.insert(result,"mp4:"..string.sub(file.name,start+length+1).."."..file.extension)
	elseif file.extension == "nsv" then
		table.insert(result,"nsv:"..string.sub(file.name,start+length+1)..".nsv")
	end
end

