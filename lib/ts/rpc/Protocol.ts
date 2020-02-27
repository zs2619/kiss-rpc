import {Int64} from "./BigInt"

export abstract class Protocol {
	protected curRead:number=0
	protected urWrite:number=0
	protected buff: ArrayBuffer=new ArrayBuffer (1024)
	protected dataView:DataView
	constructor(){
        this.dataView = new DataView(this.buff);
	}
	public abstract createProtoBuffer():Protocol 
	public abstract	writeBool( b:boolean) :boolean
	public abstract	writeByte( by:number) :boolean
	public abstract	writeInt8( i:number) :boolean
	public abstract	writeInt16( i:number) :boolean
	public abstract	writeInt32( i:number) :boolean
	public abstract	writeInt64( i:Int64) :boolean
	public abstract	writeFloat( f:number) :boolean
	public abstract	writeString(str:string) :boolean
	public abstract	writeBinary( buff:Uint8Array) :boolean
	public abstract	readBool() :boolean
	public abstract	readByte() :number
	public abstract	readInt8() :number
	public abstract	readInt16() :number
	public abstract	readInt32() :number
	public abstract	readInt64() :Int64
	public abstract	readFloat() :number
	public abstract	readString() :string
	public abstract	readBinary() :Uint8Array

	public getBuffer():Uint8Array{
		return new Uint8Array(this.buff)
	}
	public setBuffer(buff:Uint8Array){
		this.buff=buff
	}
	protected growBuff(newLen:number){
		if (newLen>this.buff.byteLength){
			let adjustLen:number=1
			//  adjust to 2 power
			while(adjustLen<newLen) {
				adjustLen*=2
			}
			let newBuff:Uint8Array= new Uint8Array(adjustLen)
			newBuff.set(new Uint8Array(this.buff))
			this.buff=newBuff
        	this.dataView=new DataView(this.buff)
		}
	}
}

export class BinaryProtocol extends Protocol implements  Protocol{
	constructor(){
		super()
	}
	public	createProtoBuffer() :Protocol{
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
	public	readBinary() :Uint8Array{
		let len=this.readInt16()
		let dataBuf : Uint8Array=new Uint8Array(Int64.dataLenght)
		for (let i=0;i<Int64.dataLenght;i++){
			dataBuf[i]=this.readByte()
		}
		return dataBuf 
	}
}