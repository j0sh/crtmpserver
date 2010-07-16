package com.rtmpd
{
	import com.rtmpd.generics.Connection;
	import com.rtmpd.generics.Logging;
	
	import flash.events.Event;
	import flash.events.SyncEvent;
	import flash.net.SharedObject;
	
	import mx.utils.ObjectUtil;

	public class Engine extends Connection
	{
		private var _tsStreams:SharedObject;
		
		public function Engine() {
			super();
		}
		
		public override function Initialize():void{
			super.Initialize();
			this.addEventListener("IsConnectedEvent",IsConnectedEvent);
		}
		
		private function IsConnectedEvent(evt:Event):void{
			if(IsConnected){
				_tsStreams=SharedObject.getRemote("tsStreamsEvents",_connection.uri,true);
				_tsStreams.addEventListener(SyncEvent.SYNC,TsStreamsEvents);
				_tsStreams.connect(_connection);
			}else{
				_tsStreams=null;
				dispatchEvent(new Event("TSStreamsEvent"));
			}
		}
		
		private function TsStreamsEvents(evt:SyncEvent):void{
			Logging.Debug("evt: "+ObjectUtil.toString(evt));
			dispatchEvent(new Event("TSStreamsEvent"));
		}
		
		[Bindable(event="TSStreamsEvent")]
		public function get TSStreams():Array{
			if(_tsStreams==null)
				return null;
			return _tsStreams.data.streams;
		}
	}
}
