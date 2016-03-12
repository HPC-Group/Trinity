	else if (type == {CommandName}Reply::Ifc::Type) {
		return reader->getSerializablePtr<{CommandName}Reply>("rep");
	}
