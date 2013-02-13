/*
 * SPA - Systematic Protocol Analysis Framework
 */

#ifndef __SPA_H__
#define __SPA_H__

#include <iostream>
#include <deque>
#include <list>
#include <map>

#include <llvm/Module.h>
#include "llvm/Instructions.h"

#include <cloud9/worker/SymbolicEngine.h>

#include <spa/StateUtility.h>
#include <spa/FilteringEventHandler.h>
#include <spa/PathFilter.h>

#define SPA_API_ANNOTATION_FUNCTION				"spa_api_entry"
#define SPA_MESSAGE_HANDLER_ANNOTATION_FUNCTION	"spa_message_handler_entry"
#define SPA_CHECKPOINT_ANNOTATION_FUNCTION		"spa_checkpoint"
#define SPA_WAYPOINT_ANNOTATION_FUNCTION		"spa_waypoint"
#define SPA_RETURN_ANNOTATION_FUNCTION			"spa_return"
#define SPA_INPUT_ANNOTATION_FUNCTION			"spa_input"
// #define SPA_SEED_ANNOTATION_FUNCTION			"spa_seed"

#define SPA_PREFIX					"spa_"
#define SPA_TAG_PREFIX				SPA_PREFIX "tag_"
#define SPA_STATE_PREFIX			SPA_PREFIX "state_"
#define SPA_INPUT_PREFIX			SPA_PREFIX "in_"
#define SPA_INIT_PREFIX				SPA_PREFIX "init_"
#define SPA_API_INPUT_PREFIX		SPA_INPUT_PREFIX "api_"
#define SPA_MESSAGE_INPUT_PREFIX	SPA_INPUT_PREFIX "msg_"
#define SPA_OUTPUT_PREFIX			SPA_PREFIX "out_"
#define SPA_API_OUTPUT_PREFIX		SPA_OUTPUT_PREFIX "api_"
#define SPA_MESSAGE_OUTPUT_PREFIX	SPA_OUTPUT_PREFIX "msg_"
#define SPA_WAYPOINTS_VARIABLE		SPA_PREFIX "waypoints"

#define SPA_HANDLERTYPE_TAG			"HandlerType"
#define SPA_MESSAGEHANDLER_VALUE	"Message"
#define SPA_APIHANDLER_VALUE		"API"
#define SPA_OUTPUT_TAG				"Output"
#define SPA_OUTPUT_VALUE			"1"
#define SPA_MSGRECEIVED_TAG			"MsgReceived"
#define SPA_MSGRECEIVED_VALUE		"1"
#define SPA_VALIDPATH_TAG			"ValidPath"
#define SPA_VALIDPATH_VALUE			"1"

#define SPA_INITVALUEID_VARIABLE	"spa_internal_initValueID"
#define SPA_HANDLERID_VARIABLE		"spa_internal_HanderID"
#define SPA_SEEDID_VARIABLE			"spa_internal_SeedID"

#define LOG_FILE_VARIABLE			"SPA_LOG_FILE"


namespace SPA {
	class SPA : public cloud9::worker::StateEventHandler, FilteringEventHandler {
	private:
		llvm::Module *module;
		llvm::Function *entryFunction;
		llvm::Instruction *initHandlerPlaceHolder;
		llvm::GlobalVariable *handlerIDVarName;
		llvm::SwitchInst *initValueSwitchInst;
		llvm::SwitchInst *entryHandlerSwitchInst;
		uint32_t initValueID;
		uint32_t entryHandlerID;
		llvm::BasicBlock *firstHandlerBB;
		llvm::BasicBlock *nextHandlerBB;
		llvm::BasicBlock *returnBB;
		std::ostream &output;
		std::set<llvm::Instruction *> checkpoints;
		std::deque<StateUtility *> stateUtilities;
		std::deque<bool> outputFilteredPaths;
		PathFilter *pathFilter;
		bool outputTerminalPaths;

		unsigned long checkpointsFound, filteredPathsFound, terminalPathsFound, outputtedPaths;

		void generateMain();
		void showStats();
		void processPath( klee::ExecutionState *state );

	public:
		SPA( llvm::Module *_module, std::ostream &_output );
		void addInitFunction( llvm::Function *fn );
		void addEntryFunction( llvm::Function *fn );
// 		void addSeedEntryFunction( unsigned int seedID, llvm::Function *fn );
		void newEntryLevel();
		void addInitialValues( std::map<llvm::Value *, std::vector<std::vector<std::pair<bool,uint8_t> > > > values );
		void addSymbolicInitialValues() { addInitialValues( std::map<llvm::Value *, std::vector<std::vector<std::pair<bool,uint8_t> > > >() ); }
		void addStateUtilityFront( StateUtility *stateUtility, bool _outputFilteredPaths ) {
			stateUtilities.push_front( stateUtility );
			outputFilteredPaths.push_front( _outputFilteredPaths );
		}
		void addStateUtilityBack( StateUtility *stateUtility, bool _outputFilteredPaths ) {
			stateUtilities.push_back( stateUtility );
			outputFilteredPaths.push_back( _outputFilteredPaths );
		}
		void setPathFilter( PathFilter *_pathFilter ) { pathFilter = _pathFilter; }
		void addCheckpoint( llvm::Instruction *instruction ) { checkpoints.insert( instruction ); }
		void setOutputTerminalPaths( bool _outputTerminalPaths ) { outputTerminalPaths = _outputTerminalPaths; }
		llvm::Function *getMainFunction() { return entryFunction; }
		void start();

		bool onStateBranching(klee::ExecutionState *state, klee::ForkTag forkTag) { return true; }
		void onStateBranched(klee::ExecutionState *kState, klee::ExecutionState *parent, int index, klee::ForkTag forkTag) {}
		void onOutOfResources(klee::ExecutionState *destroyedState) {}
		void onEvent(klee::ExecutionState *kState, unsigned int type, long int value) {}
		void onDebugInfo(klee::ExecutionState *kState, const std::string &message) {}

		void onControlFlowEvent(klee::ExecutionState *kState, cloud9::worker::ControlFlowEvent event);
		void onStateDestroy(klee::ExecutionState *kState, bool silenced);
		void onStateFiltered( klee::ExecutionState *state, unsigned int id );
	};
}

#endif // #ifndef __SPA_H__
