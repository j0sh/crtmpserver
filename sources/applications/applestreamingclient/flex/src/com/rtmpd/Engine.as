package com.rtmpd
{
	import com.rtmpd.generics.Connection;
	import com.rtmpd.generics.Logging;
	
	import flash.events.Event;
	import flash.events.EventDispatcher;
	
	import mx.controls.Alert;
	import mx.utils.ObjectUtil;

	public class Engine extends com.rtmpd.generics.Connection
	{	
		private var _streamName:String;
		private var _client:Object;
		
		public function Engine() {
			super();
		}
		
		public override function Initialize():void{
			super.Initialize();
		}
		
		public function streamAvailable(streamName:String):void{
			_streamName=streamName;
			_client.StreamAvailable(_streamName);
		}
		
		public function set Client(client:Object):void{
			_client=client;
		}
	}
}
