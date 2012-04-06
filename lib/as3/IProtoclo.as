//==============================================
/**
	@date:		2012:3:31  
	@file: 		IProtocol.as
	@author		zhangshuai
*/
//==============================================
package {

public interface IProtocol 
{
public:

	/**  Writing functions. */
	function  writeBool(value:bool):void;

	function  writeUint8( i:uint):void;
	function  writeInt8( i:int) :void;

	function  writeUInt16(i:uint) :void;
	function  writeInt16(i:int ) :void;

	function  writeUInt32( i:uint) :void;
	function  writeInt32( i:int) :void;

	//function  writeInt64( int64 i) :void;

	function  writeFloat(f:Number) :void;

	function  writeString(str:String) :void;

	/** Reading functions */
	function  readBool():Boolean;

	function  readUint8():uint;
	function  readint8():int;

	function  readUInt16():uint;
	function  readInt16():int;

	function  readUInt32():uint;
	function  readInt32():int;

	//function  readInt64():void;

	function  readFloat():Number;

	function  readString():String;
};
