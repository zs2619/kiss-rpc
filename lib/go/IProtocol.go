//==============================================
/**
	@date:		2015:8:24  
	@file: 		IProtocol.go
	@author		zhangshuai
*/
//==============================================
package rpc

type  IProtocol interface
{
	WriteMsgBegin()
	WriteMsgEnd()
	/**  Writing functions. */
	
	WriteBool(b bool)
	
	WriteUInt8( i uint8)
	WriteInt8( i int8)
	
	WriteUInt16(i uint16) 
	WriteInt16(i int16 ) 
	
	WriteUInt32( i uint32) 
	WriteInt32( i int32)
	
	WriteInt64(i int64) 
	
	WriteFloat(f float32) 
	
	WriteString(str string) 
	
	/** Reading functions */
	ReadBool() (bool)
	
	ReadUInt8() (uint8)
	ReadInt8() (int8)
	
	ReadUInt16() (uint16)
	ReadInt16() (int16)
	
	ReadUInt32() (uint32)
	ReadInt32() (int32)
	
	ReadInt64() (int64)
	
	ReadFloat() (float32)
	
	ReadString() (string)
}
