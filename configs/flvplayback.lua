configuration=
{
	daemon=false,
	pathSeparator="/",

	logAppenders=
	{
		{
			name="console appender",
			type="coloredConsole",
			level=6
		},
		{
			name="file appender",
			type="file",
			level=6,
			fileName="./logs/crtmpserver",
			fileHistorySize=10,
			fileLength=1024*1024,
			singleLine=true	
		}
	},
	
	applications=
	{
		rootDirectory="applications",
		{
			description="FLV Playback Sample",
			name="flvplayback",
			protocol="dynamiclinklibrary",
			default=true,
			aliases=
			{
				"simpleLive",
				"vod",
				"live",
				"WeeklyQuest",
				"SOSample",
				"oflaDemo",
			},
			acceptors = 
			{
				{
					ip="0.0.0.0",
					port=1935,
					protocol="inboundRtmp"
				},
				--[[{
					ip="0.0.0.0",
					port=8081,
					protocol="inboundRtmps",
					sslKey="./ssl/server.key",
					sslCert="./ssl/server.crt"
				},
				{
					ip="0.0.0.0",
					port=8080,
					protocol="inboundRtmpt"
				},]]--
				{
					ip="0.0.0.0",
					port=6666,
					protocol="inboundLiveFlv",
					waitForMetadata=true,
				},
				{
					ip="0.0.0.0",
					port=9999,
					protocol="inboundTcpTs"
				},
				--[[{
					ip="0.0.0.0",
					port=554,
					protocol="inboundRtsp"
				},]]--
			},
			externalStreams = 
			{
				--[[{
					uri="rtmp://edge01.fms.dutchview.nl/botr/bunny",
					localStreamName="test1",
					tcUrl="rtmp://edge01.fms.dutchview.nl/botr/bunny", --this one is usually required and should have the same value as the uri
				}]]--
			},
			validateHandshake=false,
			keyframeSeek=true,
			seekGranularity=1.5, --in seconds, between 0.1 and 600
			clientSideBuffer=12, --in seconds, between 5 and 30
			--generateMetaFiles=true, --this will generate seek/meta files on application startup
			--renameBadFiles=false,
			mediaFolder="./media",
			--[[authentication=
			{
				rtmp={
					type="adobe",
					encoderAgents=
					{
						"FMLE/3.0 (compatible; FMSc/1.0)",
					},
					usersFile="./configs/users.lua"
				},
				rtsp={
					usersFile="./configs/users.lua"
				}
			},]]--
		},
	}
}

