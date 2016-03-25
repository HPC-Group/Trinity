{return} RendererProxy::{CommandName_small}({Parameters}) {
	{CommandName}Cmd::RequestParams params({Parameter_Variables});
	{CommandName}Request request(params, IDGenerator::nextID(), m_remoteSessionId);
	// TODO: not sure if code below is *always* generated correctly
	m_inputChannel.sendRequest(request);
}
