package com.rpc
{
	import flash.utils.ByteArray;
	
	/**
	 * ...
	 * @author Leon
	 */
	public class Protocol implements IProtocol
	{
		public var m_sendBuff:ByteArray = new ByteArray;
		public var m_recvBuff:ByteArray = new ByteArray;
		
		public function Protocol()
		{
			m_recvBuff.endian = flash.utils.Endian.LITTLE_ENDIAN;
			m_sendBuff.endian = flash.utils.Endian.LITTLE_ENDIAN;
		}
		
		public function writeMsgBegin():void
		{
		}
		
		public function writeMsgEnd():void
		{
		}
		
		public function writeBool(value:Boolean):void
		{
			m_sendBuff.writeBoolean(value);
		}
		
		public function writeUInt8(i:uint):void
		{
			m_sendBuff.writeByte(i);
		}
		
		public function writeInt8(i:int):void
		{
			m_sendBuff.writeByte(i);
		}
		
		public function writeUInt16(i:uint):void
		{
			m_sendBuff.writeShort(i);
		}
		
		public function writeInt16(i:int):void
		{
			m_sendBuff.writeShort(i);
		}
		
		public function writeUInt32(i:uint):void
		{
			m_sendBuff.writeUnsignedInt(i);
		}
		
		public function writeInt32(i:int):void
		{
			m_sendBuff.writeInt(i);
		}
		
		public function writeFloat(f:Number):void
		{
			m_sendBuff.writeFloat(f);
		}
		
		public function writeString(str:String):void
		{
			//调试使用 gb
			//writeUInt16(str.length);
			//m_sendBuff.writeUTFBytes(str);
			//m_sendBuff.writeMultiByte(str, "gb2312");
			m_sendBuff.writeUTF(str);
		}
		public function  writeInt64(i:BigInt) :void
		{
			m_sendBuff.writeBytes(i.buffer);
		}
		
		/** Reading functions */
		public function readBool():Boolean
		{
			return m_recvBuff.readBoolean();
		}
		
		public function readUInt8():uint
		{
			return m_recvBuff.readUnsignedByte();
		}
		
		public function readInt8():int
		{
			return m_recvBuff.readByte();
		}
		
		public function readUInt16():uint
		{
			return m_recvBuff.readUnsignedShort();
		}
		
		public function readInt16():int
		{
			return m_recvBuff.readShort();
		}
		
		public function readUInt32():uint
		{
			return m_recvBuff.readUnsignedInt();
		}
		
		public function readInt32():int
		{
			return m_recvBuff.readInt();
		}
		
		//function  readInt64():void;
		
		public function readFloat():Number
		{
			return m_recvBuff.readFloat();
		}
		
		public function readString():String
		{
			//var len:uint = readUInt16();
			//if (len == 0)
				//return "";
			//return m_recvBuff.readUTFBytes(len);
			//return m_recvBuff.readMultiByte(len, "gb2312");
			return m_recvBuff.readUTF();
		}
		public function  readInt64():BigInt
		{
			var data:ByteArray=new ByteArray;
			m_recvBuff.readBytes(data, 0, 8);
			return new BigInt(data);
		}
	}
}