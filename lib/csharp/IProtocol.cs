using System;
public interface IProtocol  
{
	void  writeMsgBegin();
	void  writeMsgEnd();
	/**  Writing voids. */
	
	void  writeBool(bool value);
	
	void  writeUInt8( byte i);
	void  writeInt8( sbyte i) ;

	void  writeUInt16(ushort i) ;
	void  writeInt16(short i ) ;
	
	void  writeUInt32( uint i);
	void  writeInt32( int i) ;
	
	void  writeInt64(long i);
	
	void  writeFloat(float f);
	
	void  writeString(string str);
	
	/** Reading voids */
	bool  readBool();
	
	byte  readUInt8();
	sbyte  readInt8();
	
	ushort  readUInt16();
	short  readInt16();
	
	uint  readUInt32();
	int  readInt32();
	
	long  readInt64();
	
	float  readFloat();
	
	string  readString();
}
