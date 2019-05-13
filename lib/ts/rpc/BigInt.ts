
export class Int64{
	public static readonly dataLenght:number=8
	private dataBuf : Uint8Array=new Uint8Array(Int64.dataLenght)
	constructor(data:Uint8Array|null=null){
		if (data===null){
			for(let i=0;i<Int64.dataLenght;i++){
				this.dataBuf[i]=0
			}
		} else{
			this.dataBuf=data
		}
	}
	public isEqual(bi:Int64):boolean{
		let biArray=bi.getDataBuff()
		for(let i=0;i<Int64.dataLenght;i++){
			if (this.dataBuf[i]!==biArray[i]){
				return false
			}
		}
		return true

	}
	public toString():string{
		return ""
	}

	public getDataBuff():Uint8Array{
		return this.dataBuf
	}
}