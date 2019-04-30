
interface IProtocol {
	createProtoBuffer() :IProtocol
	writeBool( b:boolean) :boolean
	writeByte( by:number) :boolean
	writeInt8( i:number) :boolean
	writeInt16( i:number) :boolean
	writeInt32( i:number) :boolean
	writeInt64( i:number) :boolean
	writeFloat( f:number) :boolean
	writeString(str:string) :boolean
	writeBinary( buff:Uint8Array) :boolean
	readBool() :boolean
	readByte() :number
	readInt8() :number
	readInt16() :number
	readInt32() :number
	readInt64() :number
	readFloat() :number
	readString() :string
	readBinary() :Uint8Array
	curRead:number
	curWrite:number
	buff: Uint8Array
}