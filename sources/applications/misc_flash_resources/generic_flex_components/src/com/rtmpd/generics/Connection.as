package com.rtmpd.generics
{
	import flash.events.Event;
	import flash.events.EventDispatcher;
	import flash.events.NetStatusEvent;
	import flash.net.NetConnection;
	import flash.net.NetStream;
	import flash.net.Responder;
	
	public class Connection extends EventDispatcher
	{
		protected var _uri:String;
		protected var _connection:NetConnection;
		
		public function Connection() {
		}
		
		public function Initialize():void{
		}
		
		private function ConnectionStatusChanged(evt:NetStatusEvent):void{
			dispatchEvent(new Event("IsConnectedEvent"));
		}
		
		public function Connect(uri:String, ...args):void {
			_uri=uri;
			Disconnect();
			_connection=new NetConnection();
			_connection.client=this;
			_connection.addEventListener(NetStatusEvent.NET_STATUS,ConnectionStatusChanged);
			_connection.connect.apply(this, [uri].concat(args));
		}
		
		public function Disconnect():void {
			if(!IsConnected)
				return;
			_connection.close();
			_connection=null;
			dispatchEvent(new Event("IsConnectedEvent"));
		}
		
		[Bindable(event="IsConnectedEvent")]
		public function get IsConnected():Boolean{
			if(_connection==null)
				return false;
			return _connection.connected;
		}
		
		public function GetStream():NetStream{
			if(!IsConnected)
				return null;
			var result:NetStream=new NetStream(_connection);
			return result;
		}
		
		public function get FlashConnection():NetConnection{
			return _connection;
		}
		
		public function Call(functionName:String, resultCallback:Function, ... args):void{
			if(!IsConnected)
				return;
			_connection.call.apply(this, [functionName,new Responder(resultCallback)].concat(args));
		}
	}
}
