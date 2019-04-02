#pragma once

#include "builder.hpp"
#include <functional>
#include "BLI_optional.hpp"
#include "FN_tuple_call.hpp"
#include "util_wrappers.hpp"

struct PointerRNA;

namespace FN { namespace DataFlowNodes {

	typedef std::function<void (
		Builder &builder,
		const BuilderContext &ctx,
		struct bNode *bnode)> NodeInserter;

	typedef std::function<void (
		PointerRNA *socket_rna_ptr,
		Tuple &dst,
		uint index)> SocketLoader;

	typedef std::function<void (
		Builder &builder,
		const BuilderContext &ctx,
		Socket from,
		Socket to,
		struct bNodeLink *source_link)> ConversionInserter;

	typedef std::function<SharedFunction ()> FunctionGetter;

	class GraphInserters {
	private:
		SmallMap<std::string, NodeInserter> m_node_inserters;
		SmallMap<std::string, SocketLoader> m_socket_loaders;
		SmallMap<std::pair<std::string, std::string>, ConversionInserter> m_conversion_inserters;

	public:
		void reg_node_inserter(std::string idname, NodeInserter inserter);
		void reg_node_function(std::string idname, FunctionGetter getter);

		void reg_socket_loader(std::string idname, SocketLoader loader);

		void reg_conversion_inserter(
			std::string from_type,
			std::string to_type,
			ConversionInserter inserter);

		void reg_conversion_function(
			std::string from_type,
			std::string to_type,
			FunctionGetter getter);

		bool insert_node(
			Builder &builder,
			const BuilderContext &ctx,
			struct bNode *bnode);

		SmallSocketVector insert_sockets(
			Builder &builder,
			const BuilderContext &ctx,
			BSockets &bsockets);

		bool insert_link(
			Builder &builder,
			const BuilderContext &ctx,
			struct bNodeSocket *from_bsocket,
			struct bNodeSocket *to_bsocket,
			struct bNodeLink *source_link = nullptr);
	};

	GraphInserters &get_standard_inserters();

} } /* namespace FN::DataFlowNodes */