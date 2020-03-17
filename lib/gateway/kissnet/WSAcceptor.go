package kissnet

import (
	"fmt"
	"gateway/config"
	"net/http"

	"github.com/gorilla/websocket"
	"github.com/sirupsen/logrus"
)

type WSAcceptor struct {
	Acceptor
	addr   string
	server *http.Server
}

var upgrader = websocket.Upgrader{} // use default options

func NewWSAcceptor(cfg *config.GWConfig) (*WSAcceptor, error) {
	addrStr := fmt.Sprintf(":%d", cfg.Port)
	server := &http.Server{Addr: addrStr}
	return &WSAcceptor{addr: addrStr, server: server}, nil
}

func (this *WSAcceptor) Run() error {
	http.HandleFunc("/ws", func(w http.ResponseWriter, r *http.Request) {
		c, err := upgrader.Upgrade(w, r, nil)
		if err != nil {
			logrus.WithFields(logrus.Fields{
				"error": err,
			}).Fatal("WSAcceptor::Run::Upgrade error")
			return
		}
		ws := newWSConnection(c, this)
		go ws.start()
	})
	go func() {
		err := this.server.ListenAndServe()
		if err != nil {
			logrus.WithFields(logrus.Fields{
				"error": err,
			}).Warn("WSAcceptor::Run::ListenAndServe error")
			return
		}
	}()
	return nil
}

func (this *WSAcceptor) Close() error {
	this.server.Close()
	return nil
}
