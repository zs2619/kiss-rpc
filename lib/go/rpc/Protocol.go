package rpc

import (
	"bytes"
	"encoding/binary"
	"fmt"
)

type IProtocol interface {
	WriteBool(b bool)
	WriteUInt8(i uint8)
	WriteInt8(i int8)
	WriteUInt16(i uint16)
	WriteInt16(i int16)
	WriteUInt32(i uint32)
	WriteInt32(i int32)
	WriteInt64(i int64)
	WriteFloat(f float32)
	WriteString(str string)

	ReadBool() bool
	ReadUInt8() uint8
	ReadInt8() int8
	ReadUInt16() uint16
	ReadInt16() int16
	ReadUInt32() uint32
	ReadInt32() int32
	ReadInt64() int64
	ReadFloat() float32
	ReadString() string
}

type BinaryProtocol struct {
	RecvBuf *bytes.Buffer
	SendBuf *bytes.Buffer
}

/**  Writing functions. */
func (this *BinaryProtocol) WriteBool(b bool) {
	if b {
		this.WriteUInt8(1)
	} else {
		this.WriteUInt8(0)
	}
}

func (this *BinaryProtocol) WriteUInt8(i uint8) {

	binary.Write(this.SendBuf, binary.LittleEndian, i)
}

func (this *BinaryProtocol) WriteInt8(i int8) {
	binary.Write(this.SendBuf, binary.LittleEndian, i)

}

func (this *BinaryProtocol) WriteUInt16(i uint16) {

	binary.Write(this.SendBuf, binary.LittleEndian, i)
}

func (this *BinaryProtocol) WriteInt16(i int16) {

	binary.Write(this.SendBuf, binary.LittleEndian, i)
}

func (this *BinaryProtocol) WriteUInt32(i uint32) {

	binary.Write(this.SendBuf, binary.LittleEndian, i)
}

func (this *BinaryProtocol) WriteInt32(i int32) {

	binary.Write(this.SendBuf, binary.LittleEndian, i)
}

func (this *BinaryProtocol) WriteInt64(i int64) {
	binary.Write(this.SendBuf, binary.LittleEndian, i)

}

func (this *BinaryProtocol) WriteFloat(f float32) {

	binary.Write(this.SendBuf, binary.LittleEndian, f)
}

func (this *BinaryProtocol) WriteString(str string) {
	this.WriteUInt16(uint16(len(str)))
	var tmp = []uint8(str)
	err := binary.Write(this.SendBuf, binary.LittleEndian, tmp)
	if err != nil {
		fmt.Println(err)
	}
}

/** Reading functions */
func (this *BinaryProtocol) ReadBool() bool {
	b := this.ReadUInt8()
	if b == 1 {
		return true
	} else {
		return false
	}
}

func (this *BinaryProtocol) ReadUInt8() uint8 {
	var i uint8
	binary.Read(this.RecvBuf, binary.LittleEndian, &i)
	return i
}

func (this *BinaryProtocol) ReadInt8() int8 {
	var i int8
	binary.Read(this.RecvBuf, binary.LittleEndian, &i)
	return i
}

func (this *BinaryProtocol) ReadUInt16() uint16 {
	var i uint16
	binary.Read(this.RecvBuf, binary.LittleEndian, &i)
	return i
}

func (this *BinaryProtocol) ReadInt16() int16 {
	var i int16
	binary.Read(this.RecvBuf, binary.LittleEndian, &i)
	return i
}

func (this *BinaryProtocol) ReadUInt32() uint32 {
	var i uint32
	binary.Read(this.RecvBuf, binary.LittleEndian, &i)
	return i
}

func (this *BinaryProtocol) ReadInt32() int32 {
	var i int32
	binary.Read(this.RecvBuf, binary.LittleEndian, &i)
	return i
}

func (this *BinaryProtocol) ReadInt64() int64 {
	var i int64
	binary.Read(this.RecvBuf, binary.LittleEndian, &i)
	return i
}

func (this *BinaryProtocol) ReadFloat() float32 {
	var f float32
	binary.Read(this.RecvBuf, binary.LittleEndian, &f)
	return f
}

func (this *BinaryProtocol) ReadString() string {
	n := this.ReadUInt16()
	buf := make([]uint8, n)
	err := binary.Read(this.RecvBuf, binary.LittleEndian, buf)
	if err != nil {
		fmt.Println(err)
	}
	return string(buf)
}
