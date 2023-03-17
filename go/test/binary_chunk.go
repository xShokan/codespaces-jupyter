package binary_chunk

import (
)

const (
    LUA_SIGNATURE = "\x1bLua"
    LUAC_VERSION = 0x53
    LUAC_FORMAT = 0
    LUAC_DATA = "\x19\x93\r\n\x1a\n"
    CINT_SIZE = 4
    CSZIET_SIZE = 8
)

const (
    TAG_NIL = 0x00
    TAG_BOOLEAN = 0x01
    TAG_NUMBER = 0x03
    TAG_INTEGER = 0x13
    TAG_SHORT_STR = 0x04
    TAG_LONG_STR = 0x14
)

type header struct{
    signature [4]byte
    version byte
    format byte
}

type Upvalue struct {
    Instack byte
    Idx byte
}

type LocVar struct {
    VarName string
    StartPC uint32
    EndPC uint32
}

type Prototype struct {
    Source string
    LineDefined uint32
    LastLineDefined uint32
    NumParams  byte
    IsVararg byte
    MaxStackSize byte
    Code []uint32
    Constants []interface{}
    Upvalues []Upvalue
    Protos []*Prototype
    LineInfo []uint32
    LocVars []LocVar
    UpvalueNames []string
}

type binaryChunk struct{
    header header
    sizeUpvalues byte
    mainFunc *Prototype
}

func Undump(data []byte) *Prototype{
    reader := &reader{data}
    reader.checkHeader() // 校验头部
    reader.readByte() // 跳过Upvalue数量
    return reader.readProto("") // 读取函数原型
}

func main() {
    
}