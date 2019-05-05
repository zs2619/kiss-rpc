namespace rpc{

export interface IProtocol {
	createProtoBuffer() :IProtocol
	writeBool( b:boolean) :boolean
	writeByte( by:number) :boolean
	writeInt8( i:number) :boolean
	writeInt16( i:number) :boolean
	writeInt32( i:number) :boolean
	writeInt64( i:Int64) :boolean
	writeFloat( f:number) :boolean
	writeString(str:string) :boolean
	writeBinary( buff:Uint8Array|null) :boolean
	readBool() :boolean
	readByte() :number
	readInt8() :number
	readInt16() :number
	readInt32() :number
	readInt64() :Int64
	readFloat() :number
	readString() :string
	readBinary() :Uint8Array|null
}

abstract class Protocol {
	protected curRead:number=0
	protected urWrite:number=0
	protected buff: Uint8Array=new Uint8Array(1024)
	protected dataView:DataView
	constructor(){
        this.dataView = new DataView(this.buff);
	}

	public getBuffer():Uint8Array|null{
		return null
	}
	public setBuffer(buff:Uint8Array){
		this.buff=buff
	}
	protected growBuff(newLen:number){
		if (newLen>this.buff.length){
			let adjustLen:number=1
			//  adjust to 2 power
			while(adjustLen<newLen) {
				adjustLen*=2
			}
			let newBuff:Uint8Array= new Uint8Array(adjustLen)
			newBuff.set(this.buff)
			this.buff=newBuff
        	this.dataView=new DataView(this.buff)
		}
	}
}

export class BinaryProtocol extends Protocol implements  IProtocol{
	constructor(){
		super()
	}
	public	createProtoBuffer() :IProtocol{
		return new BinaryProtocol
	}
	public	writeBool( b:boolean) :boolean{
		if (b){
			this.writeInt8(1)
		} else{
			this.writeInt8(0)
		}
		return true
	}
	public	writeByte( by:number) :boolean{
		let ret=this.dataView.setUint8(this.curRead,by)
		this.curRead++
		return true
	}
	public	writeInt8( i:number) :boolean{
		let ret=this.dataView.setInt8(this.curRead,i)
		this.curRead++
		return true
	}
	public	writeInt16( i:number) :boolean{
		let ret=this.dataView.setInt16(this.curRead,i)
		this.curRead+=2
		return true
	}
	public	writeInt32( i:number) :boolean{
		let ret=this.dataView.setInt32(this.curRead,i)
		this.curRead+=4
		return true
	}
	public	writeInt64( i64:Int64) :boolean{
		let dataBuff =i64.getDataBuff()
		this.writeBinary(dataBuff)
		return true
	}
	public	writeFloat( f:number) :boolean{
		let ret=this.dataView.setFloat32(this.curRead,f)
		this.curRead+=4
		return true
	}
	public	writeString(str:string) :boolean{
		this.writeInt32(str.length)
		let buff = new TextEncoder().encode(str)
		for(let i=0;i<buff.length;i++){
			this.writeByte(buff[i])
		}
		return true
	}
	public	writeBinary( buff:Uint8Array) :boolean{
		this.writeInt32(buff.length)
		for(let i=0;i<buff.length;i++){
			this.writeByte(buff[i])
		}
		return true
	}
	public	readBool() :boolean{
		let ret=this.dataView.getInt8(this.curRead)
		this.curRead++
		if (ret==0){
			return false
		} else{
			return true
		}
	}
	public	readByte() :number{
		let ret=this.dataView.getUint8(this.curRead)
		this.curRead++
		return  ret
	}
	public	readInt8() :number{
		let ret=this.dataView.getInt8(this.curRead)
		this.curRead++
		return  ret
	}
	public	readInt16() :number{
		let ret=this.dataView.getInt16(this.curRead)
		this.curRead+=2
		return  ret
	}
	public	readInt32() :number{
		let ret=this.dataView.getInt32(this.curRead)
		this.curRead+=4
		return  ret
	}
	public	readInt64() :Int64{
		let dataBuf : Uint8Array=new Uint8Array(Int64.dataLenght)
		for (let i=0;i<Int64.dataLenght;i++){
			dataBuf[i]=this.readByte()
		}
		let ret:Int64=new Int64(dataBuf)
		return  ret 
	}
	public	readFloat() :number{
		let ret=this.dataView.getFloat32(this.curRead)
		this.curRead+=4
		return  ret
	}
	public	readString() :string{
		let len=this.readInt16()
		let dataBuf : Uint8Array=new Uint8Array(Int64.dataLenght)
		for (let i=0;i<Int64.dataLenght;i++){
			dataBuf[i]=this.readByte()
		}
		let ret = new TextDecoder("utf-8").decode(dataBuf);
		return ret 
	}
	public	readBinary() :Uint8Array|null{
		let len=this.readInt16()
		let dataBuf : Uint8Array=new Uint8Array(Int64.dataLenght)
		for (let i=0;i<Int64.dataLenght;i++){
			dataBuf[i]=this.readByte()
		}
		return dataBuf 
	}
}
}