package config

import (
	"encoding/json"
	"io/ioutil"
)

type ProxyItemConfigType struct {
	ProxyPass string `json:"proxyPass"`
}

type GWConfig struct {
	Port         int                            `json:"port"`
	AcceptorType string                         `json:"acceptorType"`
	ServiesProxy map[string]ProxyItemConfigType `json:"serviesProxy"`
}

var gGWConfig GWConfig

func LoadGWConfig() (*GWConfig, error) {
	content, err := ioutil.ReadFile("gateway.conf")
	if err != nil {
		return nil, err
	}
	err = json.Unmarshal(content, &gGWConfig)
	if err != nil {
		return nil, err
	}

	return &gGWConfig, nil
}

func GetGWConfig() *GWConfig {
	return &gGWConfig
}
