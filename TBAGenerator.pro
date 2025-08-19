TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    ../TBAGenerator.cpp \
    ../support_funcs.cpp \
    containers/Array.cpp \
    containers/BitArray.cpp \
    containers/Hash.cpp \
    containers/Map.cpp \
    containers/Set.cpp \
    util/ControlFlow.cpp \
    util/DesignStack.cpp \
    util/FileSystem.cpp \
    util/LineFile.cpp \
    util/MemPool.cpp \
    util/Message.cpp \
    util/NameSpace.cpp \
    util/Protect.cpp \
    util/RuntimeFlags.cpp \
    util/SaveRestore.cpp \
    util/Strings.cpp \
    util/TextBasedDesignMod.cpp \
    util/VerificStream.cpp \
    verilog/VeriAmsExpression.cpp \
    verilog/VeriAmsId.cpp \
    verilog/VeriAmsMisc.cpp \
    verilog/VeriAmsModuleItem.cpp \
    verilog/VeriAmsStatement.cpp \
    verilog/VeriBinarySaveRestore.cpp \
    verilog/VeriConstVal.cpp \
    verilog/VeriCopy.cpp \
    verilog/VeriCrossLanguageConversion.cpp \
    verilog/VeriExplicitStateMachine.cpp \
    verilog/VeriExpression.cpp \
    verilog/VeriId.cpp \
    verilog/VeriLibrary.cpp \
    verilog/VeriMisc.cpp \
    verilog/VeriModule.cpp \
    verilog/VeriModuleItem.cpp \
    verilog/VeriPrettyPrint.cpp \
    verilog/VeriPrintIPXACT.cpp \
    verilog/VeriReplace.cpp \
    verilog/VeriScope.cpp \
    verilog/VeriStatement.cpp \
    verilog/VeriTreeNode.cpp \
    verilog/VeriVarUsage.cpp \
    verilog/VeriVisitor.cpp \
    verilog/veri_file.cpp \
    verilog/veri_lex.cpp \
    verilog/veri_yacc.cpp \
    vhdl/VhdlBinarySaveRestore.cpp \
    vhdl/VhdlConfiguration.cpp \
    vhdl/VhdlCopy.cpp \
    vhdl/VhdlCrossLanguageConversion.cpp \
    vhdl/VhdlDeclaration.cpp \
    vhdl/VhdlExplicitStateMachine.cpp \
    vhdl/VhdlExpression.cpp \
    vhdl/VhdlIdDef.cpp \
    vhdl/VhdlMisc.cpp \
    vhdl/VhdlName.cpp \
    vhdl/VhdlPrettyPrint.cpp \
    vhdl/VhdlPrintIPXACT.cpp \
    vhdl/VhdlReplace.cpp \
    vhdl/VhdlScope.cpp \
    vhdl/VhdlSpecification.cpp \
    vhdl/VhdlStatement.cpp \
    vhdl/VhdlTreeNode.cpp \
    vhdl/VhdlUnits.cpp \
    vhdl/VhdlVarUsage.cpp \
    vhdl/VhdlVisitor.cpp \
    vhdl/vhdl_file.cpp \
    vhdl/vhdl_lex.cpp \
    vhdl/vhdl_yacc.cpp \
    synlib/SynlibElabVisitor.cpp \
    synlib/SynlibExpr.cpp \
    synlib/SynlibGroup.cpp \
    synlib/SynlibStatement.cpp \
    synlib/SynlibTreeNode.cpp \
    synlib/SynlibVisitor.cpp \
    synlib/synlib_file.cpp \
    synlib/synlib_function.cpp \
    synlib/synlib_function_lex.cpp \
    synlib/synlib_function_yacc.cpp \
    synlib/synlib_lex.cpp \
    synlib/synlib_yacc.cpp \
    verilog_nl/VeriNetlistNode.cpp \
    verilog_nl/veri_nl_file.cpp \
    verilog_nl/veri_nl_lex.cpp \
    verilog_nl/veri_nl_yacc.cpp \
    database/Att.cpp \
    database/Cell.cpp \
    database/Database_Bdmp.cpp \
    database/DesignObj.cpp \
    database/Fsm.cpp \
    database/Instance.cpp \
    database/Library.cpp \
    database/Libset.cpp \
    database/Net.cpp \
    database/Netlist.cpp \
    database/Operators.cpp \
    database/Port.cpp \
    database/PortRef.cpp \
    database/Primitives.cpp \
    database/VeriWrite.cpp \
    database/VhdlWrite.cpp
INCLUDE += commands
INCLUDE += containers
HEADERS += FileSystem.h LineFile.h Message.h DesignStack.h NameSpace.h Strings.h SaveRestore.h VerificSystem.h TextBasedDesignMod.h MemPool.h Protect.h VerificStream.h ControlFlow.h RuntimeFlags.h \
    ../support_funcs.h \
    containers/Array.h \
    containers/BitArray.h \
    containers/Hash.h \
    containers/Map.h \
    containers/Set.h \
    util/ControlFlow.h \
    util/DesignStack.h \
    util/FileSystem.h \
    util/LineFile.h \
    util/MemPool.h \
    util/Message.h \
    util/NameSpace.h \
    util/Protect.h \
    util/RuntimeFlags.h \
    util/SaveRestore.h \
    util/Strings.h \
    util/TextBasedDesignMod.h \
    util/VerificStream.h \
    util/VerificSystem.h \
    verilog/VeriAmsExpression.h \
    verilog/VeriAmsId.h \
    verilog/VeriAmsMisc.h \
    verilog/VeriAmsModuleItem.h \
    verilog/VeriAmsStatement.h \
    verilog/VeriClassIds.h \
    verilog/VeriCompileFlags.h \
    verilog/VeriConstVal.h \
    verilog/VeriCopy.h \
    verilog/VeriExplicitStateMachine.h \
    verilog/VeriExpression.h \
    verilog/VeriId.h \
    verilog/VeriLibrary.h \
    verilog/VeriMisc.h \
    verilog/VeriModule.h \
    verilog/VeriModuleItem.h \
    verilog/VeriRuntimeFlags.h \
    verilog/VeriScope.h \
    verilog/VeriStatement.h \
    verilog/VeriTreeNode.h \
    verilog/VeriVarUsage.h \
    verilog/VeriVisitor.h \
    verilog/veri_file.h \
    verilog/veri_tokens.h \
    verilog/veri_yacc.h \
    vhdl/VhdlClassIds.h \
    vhdl/VhdlCompileFlags.h \
    vhdl/VhdlConfiguration.h \
    vhdl/VhdlCopy.h \
    vhdl/VhdlDeclaration.h \
    vhdl/VhdlExplicitStateMachine.h \
    vhdl/VhdlExpression.h \
    vhdl/VhdlIdDef.h \
    vhdl/VhdlMisc.h \
    vhdl/VhdlName.h \
    vhdl/VhdlRuntimeFlags.h \
    vhdl/VhdlScope.h \
    vhdl/VhdlSpecification.h \
    vhdl/VhdlStatement.h \
    vhdl/VhdlTreeNode.h \
    vhdl/VhdlUnits.h \
    vhdl/VhdlVarUsage.h \
    vhdl/VhdlVisitor.h \
    vhdl/vhdl_file.h \
    vhdl/vhdl_tokens.h \
    vhdl/vhdl_yacc.h \
    synlib/SynlibCompileFlags.h \
    synlib/SynlibElabVisitor.h \
    synlib/SynlibExpr.h \
    synlib/SynlibGroup.h \
    synlib/SynlibRuntimeFlags.h \
    synlib/SynlibStatement.h \
    synlib/SynlibTreeNode.h \
    synlib/SynlibVisitor.h \
    synlib/synlib_file.h \
    synlib/synlib_function.h \
    synlib/synlib_function_tokens.h \
    synlib/synlib_function_yacc.h \
    synlib/synlib_tokens.h \
    synlib/synlib_yacc.h \
    verilog_nl/VeriNetlistCompileFlags.h \
    verilog_nl/VeriNetlistNode.h \
    verilog_nl/VeriNetlistRuntimeFlags.h \
    verilog_nl/veri_nl_file.h \
    verilog_nl/veri_nl_yacc.h \
    database/Att.h \
    database/Cell.h \
    database/DBCompileFlags.h \
    database/DBRuntimeFlags.h \
    database/DataBase.h \
    database/DesignObj.h \
    database/Fsm.h \
    database/Instance.h \
    database/Library.h \
    database/Libset.h \
    database/Net.h \
    database/Netlist.h \
    database/Operators.h \
    database/Port.h \
    database/PortRef.h \
    database/Primitives.h \
    database/VeriWrite.h \
    database/VhdlWrite.h
HEADERS += Array.h Hash.h Map.h Set.h BitArray.h

INCLUDE += util

HEADERS += synlib_file.h  synlib_tokens.h synlib_function_tokens.h SynlibExpr.h \
              SynlibStatement.h SynlibGroup.h SynlibTreeNode.h SynlibCompileFlags.h \
          SynlibVisitor.h SynlibElabVisitor.h SynlibRuntimeFlags.h

INCLUDE += containers database util


OTHER_FILES +=

DISTFILES +=

