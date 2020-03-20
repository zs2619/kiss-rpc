package main

import (
	"kiss/gateway/config"
	"kiss/kissnet"
	"kiss/rpc"

	"github.com/sirupsen/logrus"
)

var gRPCConnectorMgr = kissnet.RpcConnectorMgr{
	RpcConnMap: make(map[string]*kissnet.RpcConnector),
}
var gAcceptor kissnet.IAcceptor

func initLog() {
	logrus.SetFormatter(&logrus.JSONFormatter{})
}
func main() {
	initLog()
	_, err := config.LoadGWConfig()
	if err != nil {
		logrus.WithFields(logrus.Fields{
			"error": err,
		}).Fatal("LoadGWConfig error")
		return
	}

	event := rpc.NewNetEvent()
	err = gRPCConnectorMgr.Init(config.GetGWConfig())
	if err != nil {
		logrus.WithFields(logrus.Fields{
			"error": err,
		}).Fatal("connector err")
		return
	}
	logrus.Info("acceptor start")
	gAcceptor, err := kissnet.AcceptorFactory(config.GetGWConfig())
	if err != nil {
		logrus.WithFields(logrus.Fields{
			"error": err,
		}).Fatal("AcceptorFactory error")
		return
	}

	event.ScheduleTimer(1000*5, func() error {
		logrus.Info("ScheduleTimer")
		return nil
	})
	gAcceptor.Run()
	event.EventLoop()
	gAcceptor.Close()
	logrus.Info("acceptor end")
}
