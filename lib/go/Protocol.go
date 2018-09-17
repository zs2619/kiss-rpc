//==============================================
/**
@date:		2015:8:26
@file: 		Protocol.go
@author		zhangshuai
*/
//==============================================
package rpc

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"sync"
)

type Protocol struct {
	sync.Mutex //可能会在不同一个协程中
	Rpc        *RpcSession
	RecvBuf    *bytes.Buffer
	SendBuf    *bytes.Buffer
}

func (this *Protocol) WriteMsgBegin() {
	this.Lock()
	this.SendBuf.Reset()
}
func (this *Protocol) WriteMsgEnd() {

	buf := new(bytes.Buffer)
	binary.Write(buf, binary.LittleEndian, uint16(this.SendBuf.Len()))
	buf.Write(this.SendBuf.Bytes())
	this.Rpc.Send(buf)
	this.Unlock()
}

/**  Writing functions. */

func (this *Protocol) WriteBool(b bool) {
	if b {
		this.WriteUInt8(1)
	}
	this.WriteUInt8(0)
}

func (this *Protocol) WriteUInt8(i uint8) {

	binary.Write(this.SendBuf, binary.LittleEndian, i)
}

func (this *Protocol) WriteInt8(i int8) {
	binary.Write(this.SendBuf, binary.LittleEndian, i)

}

func (this *Protocol) WriteUInt16(i uint16) {

	binary.Write(this.SendBuf, binary.LittleEndian, i)

}

func (this *Protocol) WriteInt16(i int16) {

	binary.Write(this.SendBuf, binary.LittleEndian, i)

}

func (this *Protocol) WriteUInt32(i uint32) {

	binary.Write(this.SendBuf, binary.LittleEndian, i)
}

func (this *Protocol) WriteInt32(i int32) {

	binary.Write(this.SendBuf, binary.LittleEndian, i)
}

func (this *Protocol) WriteInt64(i int64) {
	binary.Write(this.SendBuf, binary.LittleEndian, i)

}

func (this *Protocol) WriteFloat(f float32) {

	binary.Write(this.SendBuf, binary.LittleEndian, f)
}

func (this *Protocol) WriteString(str string) {
	this.WriteUInt16(uint16(len(str)))
	var tmp = []uint8(str)
	err := binary.Write(this.SendBuf, binary.LittleEndian, tmp)
	if err != nil {
		fmt.Println(err)
	}
}

/** Reading functions */
func (this *Protocol) ReadBool() bool {
	b := this.ReadUInt8()
	if b == 1 {
		return true
	} else {
		return false
	}
}

func (this *Protocol) ReadUInt8() uint8 {
	var i uint8
	binary.Read(this.RecvBuf, binary.LittleEndian, &i)
	return i
}

func (this *Protocol) ReadInt8() int8 {
	var i int8
	binary.Read(this.RecvBuf, binary.LittleEndian, &i)
	return i
}

func (this *Protocol) ReadUInt16() uint16 {
	var i uint16
	binary.Read(this.RecvBuf, binary.LittleEndian, &i)
	return i
}

func (this *Protocol) ReadInt16() int16 {
	var i int16
	binary.Read(this.RecvBuf, binary.LittleEndian, &i)
	return i
}

func (this *Protocol) ReadUInt32() uint32 {
	var i uint32
	binary.Read(this.RecvBuf, binary.LittleEndian, &i)
	return i
}

func (this *Protocol) ReadInt32() int32 {
	var i int32
	binary.Read(this.RecvBuf, binary.LittleEndian, &i)
	return i
}

func (this *Protocol) ReadInt64() int64 {
	var i int64
	binary.Read(this.RecvBuf, binary.LittleEndian, &i)
	return i
}

func (this *Protocol) ReadFloat() float32 {
	var f float32
	binary.Read(this.RecvBuf, binary.LittleEndian, &f)
	return f
}

func (this *Protocol) ReadString() string {
	n := this.ReadUInt16()
	buf := make([]uint8, n)
	err := binary.Read(this.RecvBuf, binary.LittleEndian, buf)
	if err != nil {
		fmt.Println(err)
	}
	return string(buf)
}
