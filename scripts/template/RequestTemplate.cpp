	else if (type == {CommandName}Request::Ifc::Type) {
		return reader->getSerializablePtr<{CommandName}Request>("req");
	}
