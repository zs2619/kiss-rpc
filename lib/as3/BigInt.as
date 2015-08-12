package com.rpc
{
	import flash.display.InteractiveObject;
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	public class BigInt
	{
		private var m_dataBuf:ByteArray;
		
		public function BigInt(data:ByteArray = null)
		{
			m_dataBuf = new ByteArray();
			m_dataBuf.endian = Endian.LITTLE_ENDIAN;
			if (data == null)
				for (var i:int = 0; i < 8; i++)
					m_dataBuf[i] = 0;
			else
				data.readBytes(m_dataBuf, 0, 8);
		}
		
		public function isEqual(bi:BigInt):Boolean
		{
			if (this == bi)
				return true;
			
			var bEqual:Boolean = true;
			m_dataBuf.position = 0;
			bi.buffer.position = 0;
			
			for (var i:int = 0; i < 8; i++)
			{
				var byte:int = m_dataBuf.readByte();
				var byte_:int = bi.readByte();
				if ( byte != byte_)
				{
					bEqual = false;
					break;
				}
			}
			
			m_dataBuf.position = 0;
			return bEqual;
		}
		
		public function toString():String
		{
			m_dataBuf.position = 0;
			var str:String = new String();
			str += uint( m_dataBuf.readUnsignedInt() ).toString(16);
			if (str.length < 8)
				for (var i:int = str.length; i < 8; i++)
					str = "0" + str;
			var str2:String = new String();
			str2 += uint( m_dataBuf.readUnsignedInt() ).toString(16);
			if (str2.length < 8)
				for (var j:int = str2.length; j < 8; j++)
					str2 = "0" + str2;
			m_dataBuf.position = 0;
			return "0x"  + str2 + str;
		}
		
		public function get low():uint
		{
			m_dataBuf.position = 0;
			return uint( m_dataBuf.readUnsignedInt() );
		}
		
		public function get high():uint
		{
			m_dataBuf.position = 4;
			return uint( m_dataBuf.readUnsignedInt() );
		}
		
		public function readByte():int
		{
			return m_dataBuf.readByte();
		}
		
		public function get buffer():ByteArray
		{
			return m_dataBuf;
		}
	}
}