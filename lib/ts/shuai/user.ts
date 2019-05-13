import  * as rpc from "../rpc/index"
import  {role}  from "./role"
export class user
{ 
	public static readonly strFingerprint:string="58d411b316d3ad09ca4c27108e0ca7d7"
	public roleMap:Map<number,role>=new Map<number,role>()
	public items:number[]=[]
	public roles:role[]=[]
	//serialize
	public serialize( __P__:rpc.IProtocol):void 
	{ 
		__P__.writeInt32(this.roleMap.size);
		this.roleMap.forEach((value , key) =>{ 
			__P__.writeInt32(key);
			value.serialize(__P__);
		})

		__P__.writeInt32(this.items.length);
		for (let  temp of this.items)
		{
			__P__.writeInt32(temp);
		}

		__P__.writeInt32(this.roles.length);
		for (let  temp of this.roles)
		{
			temp.serialize(__P__);
		}

	}// serialize
	//deSerialize
	public deSerialize( __P__:rpc.IProtocol):void 
	{ 
		let _n_0_map:number=__P__.readInt32()
		for(let _i_0_:number=0; _i_0_<_n_0_map; _i_0_++ ){
			let tmpKey:number=0
			tmpKey=__P__.readInt32()
			let tmpValue:role=new role()
			tmpValue.deSerialize(__P__)
			this.roleMap.set(tmpKey,tmpValue)
		}

		let _n_0_array:number=__P__.readInt32()
		for( let _i_0_:number=0; _i_0_<_n_0_array; _i_0_++) {
			let tmp:number=0
			tmp=__P__.readInt32()
			this.items.push(tmp)
		}

		let _n_1_array:number=__P__.readInt32()
		for( let _i_1_:number=0; _i_1_<_n_1_array; _i_1_++) {
			let tmp:role=new role()
			tmp.deSerialize(__P__)
			this.roles.push(tmp)
		}

	}// deSerialize
}//class
