using System;
using System.Collections.Generic;
using System.Text;
public class Protocol : IProtocol 
{
	//private Encoding encode = Encoding.GetEncoding("gb2312");
	//private Encoding encode=UTF8Encoding.Default;
    private Encoding encode = Encoding.UTF8;

	public byte[] recvBuffer;
	public byte[] sendBuffer;

	public int recvPos = 0;
	public int sendPos = 0;

	public virtual void  writeMsgBegin(){}
	public virtual void  writeMsgEnd(){}
	/**  Writing voids. */

	private void write(byte[] bytes, int len)
	{
		Array.Copy(bytes, 0, sendBuffer, sendPos, len);
		sendPos+=len;
	}
	public void  writeBool(bool value)
	{
		byte[] bytes = BitConverter.GetBytes(value);
		write(bytes,1);
	}
	
	public void  writeUInt8( byte i)
	{
		sendBuffer[sendPos] = i;
		sendPos+=1;
	}
	public void  writeInt8( sbyte i)
	{
		sendBuffer[sendPos] = (byte)i;
		sendPos+=1;
	}
	
	public void  writeUInt16(ushort i)
	{
		byte[] bytes = BitConverter.GetBytes(i);
		write(bytes,2);
	}
	public void writeInt16(short i)
	{
		byte[] bytes = BitConverter.GetBytes(i);
		write(bytes,2);
	}
	
	public void  writeUInt32( uint i)
	{
		byte[] bytes = BitConverter.GetBytes(i);
		write(bytes,4);
	}
	public void  writeInt32( int i) 
	{
		byte[] bytes = BitConverter.GetBytes(i);
		write(bytes,4);
	}
	
	public void  writeInt64(long i)
	{
		byte[] bytes = BitConverter.GetBytes(i);
		write(bytes,8);
	}
	
	public void  writeFloat(float f)
	{
		byte[] bytes = BitConverter.GetBytes(f);
		write(bytes,4);
	}

	public void writeString(string str)
	{
		byte[] byteData = encode.GetBytes(str);
		byte[] byteLen = BitConverter.GetBytes((short)(byteData.Length));
		write(byteLen,2);
		write(byteData,byteData.Length);
	}
	
	/** Reading voids */
	public bool  readBool()
	{
		bool temp = BitConverter.ToBoolean(recvBuffer, recvPos);
		recvPos+=1;
		return temp;
	}
	
	public byte  readUInt8()
	{
		byte temp = recvBuffer[recvPos];
		recvPos+=1;
		return temp;
	}
	public sbyte  readInt8()
	{
		sbyte temp = (sbyte)(recvBuffer[recvPos]);
		recvPos+=1;
		return temp;
	}
	
	public ushort readUInt16()
	{
		ushort temp = BitConverter.ToUInt16(recvBuffer, recvPos);
		recvPos+=2;
		return temp;
	}
	public short readInt16()
	{
		short temp = BitConverter.ToInt16(recvBuffer, recvPos);
		recvPos+=2;
		return temp;
	}
	
	public uint  readUInt32()
	{
		uint temp = BitConverter.ToUInt32(recvBuffer, recvPos);
		recvPos+=4;
		return temp;
	}
	public int  readInt32()
	{
		int temp = BitConverter.ToInt32(recvBuffer, recvPos);
		recvPos+=4;
		return temp;
	}
	
	public long  readInt64()
	{
		long temp = BitConverter.ToInt64(recvBuffer, recvPos);
		recvPos+=8;
		return temp;
	}
	
	public float  readFloat()
	{
		float temp = BitConverter.ToSingle(recvBuffer, recvPos);
		recvPos+=4;
		return temp;
	}

	public string readString()
	{
		short strlen = readInt16();
        string temp = encode.GetString(recvBuffer, recvPos, strlen);
		recvPos+=strlen;
		return temp;
	}
}
