package shuai
const (
	Male =iota 
	Famale
)
var (
	SexTypeS2I= map[string] int16 {
	"Male" : Male,
	"Famale" : Famale,
	}
	SexTypeI2S= map[int16] string {
	Male : "Male",
	Famale : "Famale",
	}
)
