	case VclType::{VclType}: {
		return mocca::make_unique<{CommandName}Hdl>(static_cast<const {CommandName}Request&>(request), session);
		break;
	}
