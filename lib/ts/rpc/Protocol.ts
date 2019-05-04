namespace rpc{

export interface IProtocol {
	createProtoBuffer() :IProtocol
	writeBool( b:boolean) :boolean
	writeByte( by:number) :boolean
	writeInt8( i:number) :boolean
	writeInt16( i:number) :boolean
	writeInt32( i:number) :boolean
	writeInt64( i:number) :boolean
	writeFloat( f:number) :boolean
	writeString(str:string) :boolean
	writeBinary( buff:Uint8Array|null) :boolean
	readBool() :boolean
	readByte() :number
	readInt8() :number
	readInt16() :number
	readInt32() :number
	readInt64() :number
	readFloat() :number
	readString() :string
	readBinary() :Uint8Array|null
}

abstract class Protocol {
	public  curRead:number=0
	public	curWrite:number=0
	private buff: Uint8Array|null=null
}

export class BinaryProtocol extends Protocol implements  IProtocol{
	constructor(){
		super()
	}
public	createProtoBuffer() :IProtocol{
	return new BinaryProtocol
}
public	writeBool( b:boolean) :boolean{
	return true
}
public	writeByte( by:number) :boolean{
	return true
}
public	writeInt8( i:number) :boolean{
	return true
}
public	writeInt16( i:number) :boolean{
	return true
}
public	writeInt32( i:number) :boolean{
	return true
}
public	writeInt64( i:number) :boolean{
	return true
}
public	writeFloat( f:number) :boolean{
	return true
}
public	writeString(str:string) :boolean{
	return true
}
public	writeBinary( buff:Uint8Array) :boolean{
	return true
}
public	readBool() :boolean{
	return true
}
public	readByte() :number{
	return  0
}
public	readInt8() :number{
	return  0
}
public	readInt16() :number{
	return  0
}
public	readInt32() :number{
	return  0
}
public	readInt64() :number{
	return  0
}
public	readFloat() :number{
	return  0
}
public	readString() :string{
	return  ""
}
public	readBinary() :Uint8Array|null{
	return  null
}

}
}