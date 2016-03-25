{return} IOSessionProxy::{CommandName_small}({Parameters}) const {
	{CommandName}Cmd::RequestParams params({Parameter_Variables});
	{CommandName}Request request(params, IDGenerator::nextID(), m_remoteSid);
	auto reply = sendRequestChecked(m_inputChannel, request);
	// TODO: not sure if return works automatically! 
	return reply->getParams().{CommandName_small}();
}
