package rpc

import (
	"bytes"
	"encoding/binary"
	"fmt"
)

type IProtocol interface {
	CreateProtoBuffer() IProtocol
	GetBuffer() *bytes.Buffer
	SetBuffer(buff *bytes.Buffer)

	WriteBool(b bool)
	WriteByte(by byte)
	WriteInt8(i int8)
	WriteInt16(i int16)
	WriteInt32(i int32)
	WriteInt64(i int64)
	WriteFloat(f float32)
	WriteString(str string)
	WriteBinary(b []byte)

	ReadBool() bool
	ReadByte() byte
	ReadInt8() int8
	ReadInt16() int16
	ReadInt32() int32
	ReadInt64() int64
	ReadFloat() float32
	ReadString() string
	ReadBinary() []byte
}

type IProtocolFactory interface {
	NewProtocol() IProtocol
}

type BinaryProtocolFactory struct {
}

func (this BinaryProtocolFactory) NewProtocol() IProtocol {
	return &BinaryProtocol{buff: new(bytes.Buffer)}
}

type BinaryProtocol struct {
	buff *bytes.Buffer
}

func (this *BinaryProtocol) GetBuffer() *bytes.Buffer {
	return this.buff
}
func (this *BinaryProtocol) SetBuffer(buff *bytes.Buffer) {
	this.buff=buff
}

func (this *BinaryProtocol) CreateProtoBuffer() IProtocol {
	return &BinaryProtocol{buff: new(bytes.Buffer)}
}

/**  Writing functions. */
func (this *BinaryProtocol) WriteBool(b bool) {
	if b {
		this.WriteInt8(1)
	} else {
		this.WriteInt8(0)
	}
}

func (this *BinaryProtocol) WriteByte(by byte) {
	binary.Write(this.buff, binary.LittleEndian, by)
}

func (this *BinaryProtocol) WriteInt8(i int8) {
	binary.Write(this.buff, binary.LittleEndian, i)
}

func (this *BinaryProtocol) WriteInt16(i int16) {

	binary.Write(this.buff, binary.LittleEndian, i)
}

func (this *BinaryProtocol) WriteInt32(i int32) {

	binary.Write(this.buff, binary.LittleEndian, i)
}

func (this *BinaryProtocol) WriteInt64(i int64) {
	binary.Write(this.buff, binary.LittleEndian, i)

}

func (this *BinaryProtocol) WriteFloat(f float32) {

	binary.Write(this.buff, binary.LittleEndian, f)
}

func (this *BinaryProtocol) WriteString(str string) {
	this.WriteInt32(int32(len(str)))
	var tmp = []uint8(str)
	err := binary.Write(this.buff, binary.LittleEndian, tmp)
	if err != nil {
		fmt.Println(err)
	}
}

func (this *BinaryProtocol) WriteBinary(buff []byte) {
	this.WriteInt32(int32(len(buff)))
	err := binary.Write(this.buff, binary.LittleEndian, buff)
	if err != nil {
		fmt.Println(err)
	}
}

/** Reading functions */
func (this *BinaryProtocol) ReadBool() bool {
	b := this.ReadInt8()
	if b == 1 {
		return true
	} else {
		return false
	}
}

func (this *BinaryProtocol) ReadByte() byte {
	var by byte
	binary.Read(this.buff, binary.LittleEndian, &by)
	return by
}

func (this *BinaryProtocol) ReadInt8() int8 {
	var i int8
	binary.Read(this.buff, binary.LittleEndian, &i)
	return i
}

func (this *BinaryProtocol) ReadInt16() int16 {
	var i int16
	binary.Read(this.buff, binary.LittleEndian, &i)
	return i
}

func (this *BinaryProtocol) ReadInt32() int32 {
	var i int32
	binary.Read(this.buff, binary.LittleEndian, &i)
	return i
}

func (this *BinaryProtocol) ReadInt64() int64 {
	var i int64
	binary.Read(this.buff, binary.LittleEndian, &i)
	return i
}

func (this *BinaryProtocol) ReadFloat() float32 {
	var f float32
	binary.Read(this.buff, binary.LittleEndian, &f)
	return f
}

func (this *BinaryProtocol) ReadString() string {
	n := this.ReadInt32()
	buf := make([]uint8, n)
	err := binary.Read(this.buff, binary.LittleEndian, buf)
	if err != nil {
		fmt.Println(err)
	}
	return string(buf)
}

func (this *BinaryProtocol) ReadBinary() []byte {
	n := this.ReadInt32()
	buf := make([]byte, n)
	err := binary.Read(this.buff, binary.LittleEndian, buf)
	if err != nil {
		fmt.Println(err)
	}
	return buf
}
