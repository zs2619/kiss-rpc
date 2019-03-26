package rpc

type IEventHandler interface {
	Dispatch(m *RpcMsg) bool
	Invoke(m *RpcMsg) int
}
