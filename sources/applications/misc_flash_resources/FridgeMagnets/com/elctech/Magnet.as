//	=======================================
// 	Cary Dunn <cdunn@elctech.com>
//	ELC Technologies http://www.elctech.com
//

package com.elctech
{
	import flash.display.MovieClip;
	import flash.display.DisplayObject;
	import flash.display.DisplayObjectContainer;
	import flash.geom.Rectangle;
  import flash.events.*;
	import flash.display.Stage;
	import flash.text.TextField;
	import flash.net.*;
	import flash.utils.Timer;
	import caurina.transitions.Tweener;
	
	public class Magnet extends MovieClip
	{
		private var _id:uint = 0;
		private var _so = null;
		private var _nc:NetConnection = null;
		private var _l:String = null;
		static internal var dragging:Boolean = false;
		static const MAGNET_COLORS:Array = [0xFF3333, 0x3399FF, 0x339966, 0x993399, 0xFF9933]
		
		static internal const ELASTICITY:Number = .5;
		static internal const FRICTION:Number = .5;
		private var ax:Number = 0;
		private var ay:Number = 0;
    private var vx:Number = 0;
		private var vy:Number = 0;
		
		public function Magnet(l:String, nc:NetConnection, i:uint):void
		{
			this.letter.text = l;
			this.letter.textColor = MAGNET_COLORS[Math.round(Math.random()*(MAGNET_COLORS.length-1))];
			this.x = Math.round(Math.random()*390)+67;
			this.y = Math.round(Math.random()*260)+20;
			trace("New Magnet " + l);
			
			_l = l;
			_nc = nc;
			_id = i;
			
			_so = SharedObject.getRemote("letter_"+_id, _nc.uri, true);
			_so.addEventListener(SyncEvent.SYNC,syncEventHandler);
			_so.connect(_nc);
			
			this.buttonMode = true;
			
			this.addEventListener(MouseEvent.MOUSE_DOWN, function(evt:Event):void {
				dragging = true;
				stage.addEventListener(MouseEvent.MOUSE_UP, stopDragging);
				evt.target.parent.addEventListener(Event.ENTER_FRAME, updatePosition);
			});
			
			this.addEventListener(MouseEvent.MOUSE_UP, stopDragging);
		}
		
		private function stopDragging(evt:Event):void
		{
			this.removeEventListener(Event.ENTER_FRAME, updatePosition);
			stage.removeEventListener(MouseEvent.MOUSE_UP, stopDragging);
			dragging = false;
		}
		
		private function updatePosition(evt:Event):void
		{
      ax = (stage.mouseX-this.x+Math.random()*5)*ELASTICITY;
      ay = (stage.mouseY-this.y)*ELASTICITY;
      vx += ax;
      vy += ay;
      vx *= FRICTION;
      vy *= FRICTION;
      this.x += vx;
      this.y += vy;
      this.scaleX = (100-Math.abs(ax)*2)/100;
      this.scaleY = (100-Math.abs(ay)*2)/100;
      this.rotation = ax*3;

			_so.setProperty("currentPosition", {x: evt.target.x, y: evt.target.y});
		}
		
		private function syncEventHandler(evt:SyncEvent):void
		{
			trace("Sync for letter "+_l);
			if(dragging){ return; }
			
			Tweener.addTween(this, {x:_so.data.currentPosition.x, y:_so.data.currentPosition.y, time:0.5, transition:"easeOutBack"});
		}
	}
}