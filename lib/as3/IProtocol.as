//==============================================
/**
	@date:		2012:3:31  
	@file: 		IProtocol.as
	@author		zhangshuai
*/
//==============================================
package com.rpc
{
	public interface IProtocol 
	{
		function  writeMsgBegin():void;
		function  writeMsgEnd():void;
		/**  Writing functions. */
	
		function  writeBool(value:Boolean):void;

		function  writeUInt8( i:uint):void;
		function  writeInt8( i:int) :void;

		function  writeUInt16(i:uint) :void;
		function  writeInt16(i:int ) :void;

		function  writeUInt32( i:uint) :void;
		function  writeInt32( i:int) :void;

		function  writeInt64(i:BigInt) :void;

		function  writeFloat(f:Number) :void;

		function  writeString(str:String) :void;

		/** Reading functions */
		function  readBool():Boolean;

		function  readUInt8():uint;
		function  readInt8():int;

		function  readUInt16():uint;
		function  readInt16():int;

		function  readUInt32():uint;
		function  readInt32():int;

		function  readInt64():BigInt;

		function  readFloat():Number;

		function  readString():String;
	};
};
