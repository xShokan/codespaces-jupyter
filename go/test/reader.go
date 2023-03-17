package binchunk

import (
	"encoding/binary"
	"math"
)

type reader struct{
	data []byte
}

