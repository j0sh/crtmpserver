package com.rtmpd.generics
{
	import com.rtmpd.generics.ui.components.LogsWindow;
	import flash.display.DisplayObject;
	import mx.managers.PopUpManager;
	
	public class Logging
	{
		private static var DEBUG:int=0;
		private static var INFO:int=1;
		private static var WARN:int=2;
		private static var ERROR:int=3;

		[Bindable]
		public static var LogMessages:Array=new Array;

		
		private static function LogMessage(lvl:int, msg:String):void{
			LogMessages.push({level:lvl, message:msg});
		}

		public static function Debug(msg:String):void{
			LogMessage(DEBUG,msg);
		}

		public static function Error(msg:String):void{
			LogMessage(ERROR,msg);
		}
	}
}

