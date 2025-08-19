
#include "./containers/Array.h"          // Make dynamic array class Array available
#include "./containers/Map.h"            // Make associated hash table class Map available

#include "./util/Message.h"        // Make message handlers available, not used in this example

#include "./verilog/veri_file.h"      // Make Verilog reader available

#include "./verilog/VeriModule.h"     // Definition of a VeriModule and VeriPrimitive
#include "./verilog/VeriId.h"         // Definitions of all identifier definition tree nodes
#include "./verilog/VeriExpression.h" // Definitions of all verilog expression tree nodes
#include "./verilog/VeriModuleItem.h" // Definitions of all verilog module item tree nodes
#include "./verilog/VeriStatement.h"  // Definitions of all verilog statement tree nodes
#include "./verilog/VeriMisc.h"       // Definitions of all extraneous verilog tree nodes (ie. range, path, strength, etc...)
#include "./verilog/VeriScope.h"      // Symbol table of locally declared identifiers
#include "./verilog/VeriLibrary.h"    // Definition of VeriLibrary
#include "./verilog/veri_yacc.h"

#include <string.h>
#include <vector>
#include <queue>
#include "support_funcs.h"

#ifdef VERIFIC_NAMESPACE
using namespace Verific ;
#endif

struct Port {
    std::string name;
    std::string direction;
    std::string type;
    std::string bus_size;
    std::queue<char> test_queue;
    bool isClock = false;
};

struct Clock {
    std::string name;
    int period;
};

extern std::string checkAndReturnBusDimension(char *busName);
std::vector<Clock> extractClocksList(std::string);
int getTestVectors(std::string fileName,std::vector<Port> &portList);
int getBusSize(Port bus);


int main(int argc, char **argv)
{

    std::string file_nm;
    std::string tv_file;

    //--------------------------------------------------------------
    // PARSE ARGUMENTS
    //--------------------------------------------------------------
    const char *file_name = 0 ;
    std::string clksString;
    std::vector<Clock> allClocksList;

    for (int i = 1; i < argc; i++) {
        if (Strings::compare(argv[i], "-o")) {
            i++ ;
            file_name = (i < argc) ? argv[i]: 0 ;
            continue ;
        } else if (Strings::compare(argv[i], "-i")) {
            i++ ;
            file_nm = (i < argc) ? argv[i]: 0 ;
            continue ;
        } else if (Strings::compare(argv[i], "-clks")) {
            int j = i;
            //j++ ;
            do {
                j++ ;
                clksString += (i < argc) ? argv[j]: "" ;
            }
            while(clksString.back()!='}');
            i = j;
            continue ;
        } else if (Strings::compare(argv[i], "-testvec")) {
            i++ ;
            tv_file = (i < argc) ? argv[i]: 0 ;
            continue ;
        }
    }
    if(argc==1)
    {
        Message::PrintLine("Usage: Auto Testbench generator:\n");
        Message::PrintLine("         -i      <input Verilog IP file>\n");
        Message::PrintLine("         -o     <generated tb file>\n") ;
        Message::PrintLine("         -clks {list of clocks} <input ports defined as clocks and periods in ns>\n") ;
        Message::PrintLine("            Example -clks {clk1 nanosec1,clk2 nanosec2...}\n") ;
        Message::PrintLine("         -testvec <Input testvectors file> \n") ;
        return 1 ;
    }

    if(file_nm.empty()) {
        Message::PrintLine("Input file is missing!") ;
        return 1 ;
    }

    allClocksList = extractClocksList(clksString);

    if (!veri_file::Analyze(file_nm.c_str(), veri_file::SYSTEM_VERILOG)) return 2 ;

    // Get the list of top modules
    Array *top_mod_array = veri_file::GetTopModules() ;
    if (!top_mod_array) {
        // If there is no top level module then issue error
        Message::Error(0,"Cannot find any top module. Check for recursive instantiation") ;
        return 4 ;
    }
    VeriModule *module  = (VeriModule *) top_mod_array->GetFirst() ; // Get the first top level module
    delete top_mod_array ; top_mod_array = 0 ; // Cleanup, it is not required anymore

    // Just to see that this is a module, and not a primitive
    VeriIdDef *module_id = (module) ? module->Id() : 0 ;
    if(!module_id) {
        Message::Error(0, "module should always have an 'identifier'.") ;
    }

    if (module_id->IsUdp()) {
        /* This is a Verilog UDP, a primitive */
    } else {
        /* This is a module */
    }

    // Iterate through the module's list of ports ('95 style port or ANSI port declarations)
    Array *ports = module->GetPortConnects() ;
    VeriExpression *port ;
    unsigned i ;
    std::string topModule;
    std::vector<Port> inputPortList;
    std::vector<Port> outputPortList;
    std::vector<Port> inoutPortList;
    std::vector<Port> allPortList;
    std::string tbFileString;


    printf("\n####################################################\n");
    printf("Top module\n#################################################### \n%s\n",module_id->Name());
    printf("####################################################\n\n");

    topModule= module_id->Name();
    FOREACH_ARRAY_ITEM(ports, i, port) {
        if (!port) continue ; // Check for NULL pointer

        switch(port->GetClassId()) {
        case ID_VERIANSIPORTDECL:
        {
            VeriAnsiPortDecl *ansi_port = static_cast<VeriAnsiPortDecl*>(port) ;
            if(!ansi_port)
                continue;
            // eg. :
            // input reg [5:0] a, b, c ...

            // Get data type for this declaration
            VeriDataType *data_type = ansi_port->GetDataType() ; // VeriDataType includes 'type' (VERI_REG, VERI_WIRE, VERI_STRUCT etc), array dimension(s), signing ...
            if(!data_type) continue;
            unsigned port_dir = ansi_port->GetDir() ; // a token : VERI_INPUT, VERI_OUTPUT or VERI_INOUT..
            unsigned j ;
            char *portDir;
            Port portId;
            if(port_dir==VERI_INPUT) {
                portDir="Input";
                portId.direction = "input";
            }
            else if(port_dir==VERI_OUTPUT) {
                portDir="Output";
                portId.direction = "output";
            }
            else if(port_dir==VERI_INOUT) {
                portDir="Inout";
                portId.direction = "inout";
            }
            else
                portDir="Unknown dir";
            std::string portDirString = portDir;

            VeriIdDef *port_id ;
            // Iterate through all ids declared in this Ansi port decl
            FOREACH_ARRAY_ITEM(ansi_port->GetIds(), j, port_id) {
                if (!port_id) continue ;
                char *port_name = const_cast<char *>(port_id->Name());
                for (std::vector<Clock>::iterator it = allClocksList.begin() ; it != allClocksList.end(); ++it) {
                    if((*it).name ==portId.name)
                        portId.isClock = true;
                }
                std::string busDim=checkAndReturnBusDimension(port_name);
                if(!busDim.empty()) {
                    portId.bus_size = busDim;
                }
                unsigned port_dir   = port_id->Dir() ;
                if(port_dir==VERI_INPUT) {
                    portDir="Input";
                    portId.direction = "input";
                }
                else if(port_dir==VERI_OUTPUT) {
                    portDir="Output";
                    portId.direction = "output";
                }
                else if(port_dir==VERI_INOUT) {
                    portDir="Inout";
                    portId.direction = "inout";
                }
                else
                    portDir="Unknown dir";

                port_name="";
                char *port_type="";
                if ( data_type->GetType() == VERI_REAL) //port is REAL
                    port_type="real";
                else if ( data_type->GetType() == VERI_WIRE) //port is WIRE
                    port_type="wire";
                else if ( data_type->GetType() == VERI_LOGIC) //port is WIRE
                    port_type="logic";
                else if ( data_type->GetType() == VERI_REG) //port is IXS_TYPE
                    port_type="reg";
                else if ( data_type->GetType() == VERI_TRI) //port is VERI_TRI
                    port_type="tri";
                else if ( data_type->GetType() == VERI_WAND) //port is VERI_WAND
                    port_type="wand";
                else if ( data_type->GetType() == VERI_TRIAND)
                    port_type="triand";
                else if ( data_type->GetType() == VERI_WOR)
                    port_type="wor";
                else if ( data_type->GetType() == VERI_TRIOR)
                    port_type="trior";
                else if ( data_type->GetType() == VERI_TRIREG)
                    port_type="trireg";
                else if ( data_type->GetType() == VERI_TRI0)
                    port_type="tri0";
                else if ( data_type->GetType() == VERI_TRI1)
                    port_type="tri1";
                else if ( data_type->GetType() == VERI_UWIRE)
                    port_type="uwire";
                else if ( data_type->GetType() == VERI_SUPPLY0)
                    port_type="supply0";
                else if ( data_type->GetType() == VERI_SUPPLY1)
                    port_type="supply1";
                else if ( data_type->GetType() == VERI_INTEGER)
                    port_type="integer";
                else if ( data_type->GetType() == VERI_INT)
                    port_type="int";
                else if ( data_type->GetType() == VERI_BYTE)
                    port_type="byte";
                else if ( data_type->GetType() == VERI_SHORTINT)
                    port_type="shortint";
                else if ( data_type->GetType() == VERI_LONGINT)
                    port_type="longint";
                else if ( data_type->GetType() == VERI_BIT)
                    port_type="bit";
                else if ( data_type->GetType() == VERI_SHORTREAL)
                    port_type="shortreal";

                else {
                    port_type=const_cast<char *> (data_type->GetName());
                    if(!port_type)
                        port_type="Unknown";
                }

                portId.type = port_type;
                allPortList.push_back(portId);

            }
            break ;
        }

        case ID_VERIIDREF:
        {
            VeriIdRef *id_ref = static_cast<VeriIdRef*>(port) ;
            if(!id_ref)
                continue;

            // Get the resolved identifier definition that is referred here
            VeriIdDef *id = id_ref->FullId() ;
            char *portDir;
            Port portId;
            char *port_name = const_cast<char *>(id->Name());
            portId.name = port_name;

            for (std::vector<Clock>::iterator it = allClocksList.begin() ; it != allClocksList.end(); ++it) {
                if((*it).name ==portId.name)
                    portId.isClock = true;
            }

            std::string busDim=checkAndReturnBusDimension(port_name);
            if(!busDim.empty()) {
                portId.bus_size = busDim;
            }


            port_name="";
            unsigned is_port    = id->IsPort() ;  // Should return true in this case
            unsigned port_dir   = id->Dir() ;     // returns VERI_INPUT, VERI_OUTPUT, VERI_INOUT
            if(port_dir==VERI_INPUT) {
                portDir="Input";
                portId.direction = "input";
            }
            else if(port_dir==VERI_OUTPUT) {
                portDir="Output";
                portId.direction = "output";
            }
            else if(port_dir==VERI_INOUT) {
                portDir="Inout";
                portId.direction = "inout";
            }
            else
                portDir="Unknown dir";
            std::string portDirString = portDir;
            unsigned port_type  = id->Type() ;    // returns VERI_WIRE, VERI_REG, etc ...
            char *port_typeName="";
            if ( port_type == VERI_REAL) //port is REAL
                port_typeName="real";
            else if ( port_type == VERI_WIRE) //port is WIRE
                port_typeName="wire";
            else if (port_type == VERI_LOGIC) //port is WIRE
                port_typeName="logic";
            else if ( port_type == VERI_REG) //port is IXS_TYPE
                port_typeName="reg";
            else if ( port_type == VERI_TRI) //port is VERI_TRI
                port_typeName="tri";
            else if ( port_type == VERI_WAND) //port is VERI_WAND
                port_typeName="wand";
            else if (port_type == VERI_TRIAND)
                port_typeName="triand";
            else if ( port_type == VERI_WOR)
                port_typeName="wor";
            else if ( port_type == VERI_TRIOR)
                port_typeName="trior";
            else if ( port_type == VERI_TRIREG)
                port_typeName="trireg";
            else if ( port_type == VERI_TRI0)
                port_typeName="tri0";
            else if ( port_type == VERI_TRI1)
                port_typeName="tri1";
            else if ( port_type == VERI_UWIRE)
                port_typeName="uwire";
            else if ( port_type == VERI_SUPPLY0)
                port_typeName="supply0";
            else if ( port_type == VERI_SUPPLY1)
                port_typeName="supply1";
            else if ( port_type == VERI_INTEGER)
                port_typeName="integer";
            else if ( port_type == VERI_INT)
                port_typeName="int";
            else if ( port_type == VERI_BYTE)
                port_typeName="byte";
            else if ( port_type == VERI_SHORTINT)
                port_typeName="shortint";
            else if ( port_type == VERI_LONGINT)
                port_typeName="longint";
            else if ( port_type == VERI_BIT)
                port_typeName="bit";
            else if ( port_type == VERI_SHORTREAL)
                port_typeName="shortreal";
            else {
                VeriAnsiPortDecl *ansi_port = static_cast<VeriAnsiPortDecl*>(port) ;
                if(!ansi_port)
                    continue;
                VeriDataType *data_type = ansi_port->GetDataType() ;
                if(!data_type)
                    continue;
                port_typeName=const_cast<char *> ( data_type->GetName());
                if(!port_typeName /*|| !strstr(typesList,port_type)*/)
                    port_typeName="Unknown";
            }

            portId.type = port_typeName;
            allPortList.push_back(portId);
            break ;
        }
        default:
            Message::Error(port->Linefile(),"unknown port found") ;
        }
    }

    getTestVectors(tv_file,allPortList);

    FILE * pTBFile;
    if(!file_name)
        pTBFile = fopen ("exportTB.v","w");
    else {
        pTBFile = fopen (file_name,"w");
    }
    if(!pTBFile)
    {
        printf("Error in export file open\n") ;
        return 1;
    }

    tbFileString = "`timescale 1 ns /  100 ps\n";
    tbFileString =tbFileString + "module " + topModule + ";\n";
    for (std::vector<Port>::iterator it = allPortList.begin() ; it != allPortList.end(); ++it) {
        if((*it).bus_size.empty())
            tbFileString = tbFileString+ (*it).type +"  " + (*it).name + "; \n";
        else
            tbFileString = tbFileString+ (*it).type +"  " +(*it).bus_size +" " + (*it).name + "; \n";
    }
    tbFileString = tbFileString + "\n\n";

    tbFileString = tbFileString + "initial\n   begin\n";

    tbFileString = tbFileString + "  $display(\"\\t\\ttime,";
    for (std::vector<Port>::iterator it = allPortList.begin() ; it != allPortList.end(); ++it) {
        tbFileString = tbFileString + "  \\t"+(*it).name;
    }
    tbFileString = tbFileString +"\");\n";

    std::string percSign = "%%";
    tbFileString = tbFileString + "  $monitor(\"" +percSign+"d,\\t\%b,";
    for (int i =1; i<(int)allPortList.size() ; ++i) {
        tbFileString = tbFileString + "  \\t\%b,";
    }
    tbFileString = tbFileString +"\\t" +percSign+"d\",$time,";
    for (std::vector<Port>::iterator it = allPortList.begin() ; it != allPortList.end(); ++it) {
        tbFileString = tbFileString + "  \\t"+(*it).name;
        if((*it).name !=(allPortList.back()).name)
            tbFileString = tbFileString + ",";
    }
    tbFileString = tbFileString +"\");\n";

    tbFileString = tbFileString + " $dumpfile (\"" + topModule + ".vcd\");\n";
    tbFileString = tbFileString + " $dumpvars;\n";
    tbFileString = tbFileString + "end\n";
    tbFileString = tbFileString + "\n\n";

    tbFileString = tbFileString + "initial\n   begin\n";

    for (std::vector<Port>::iterator it = allPortList.begin() ; it != allPortList.end(); ++it) {
        if((*it).direction !="output")
            tbFileString = tbFileString+"   " +(*it).name +" =0;\n";
    }
    bool vecQueueIsEmpty = false;
    do {
        for (std::vector<Port>::iterator it = allPortList.begin() ; it != allPortList.end(); ++it) {
            // note that ports with direction of type OUTPUT are not supposed to have assigned values!
            // enabled it here if you have VPI procedures and need to check the outputs from the HDL simulators with these values from test-vec files!
            if(/*(*it).direction !="output" &&*/ !(*it).isClock)  {

                if(!(*it).bus_size.empty()) {
                    std::string busVector;
                    int busSize = getBusSize(*it);
                    if((*it).test_queue.empty())
                        continue;
                    for(busSize;busSize>0;--busSize) {
                        busVector+= (*it).test_queue.front();
                        (*it).test_queue.pop();
                        if((*it).test_queue.empty())
                            vecQueueIsEmpty = true;
                    }
                    tbFileString = tbFileString+"#10   " +(*it).name +" =" +std::to_string(getBusSize(*it))+"'b"+ busVector+";\n";
                } else {


                    tbFileString = tbFileString+"#10   " +(*it).name +" =" + (*it).test_queue.front()+";\n";
                    (*it).test_queue.pop();
                    if((*it).test_queue.empty())
                        vecQueueIsEmpty = true;
                }

            }
        }
    }
    while(!vecQueueIsEmpty);
    tbFileString = tbFileString + "#10  $finish;\n";
    tbFileString = tbFileString + "end\n";
    tbFileString = tbFileString + "\n\n";

    //if clock and frequency
    for (std::vector<Clock>::iterator it = allClocksList.begin() ; it != allClocksList.end(); ++it) {
        tbFileString = tbFileString + "always\n";
        tbFileString = tbFileString + "#"+ std::to_string((*it).period) + " " +  (*it).name +" = ~" +  (*it).name+"\n";
    }
    tbFileString = tbFileString + "\n\n";


    tbFileString = tbFileString+topModule + "  U0 (\n";
    for (std::vector<Port>::iterator it = allPortList.begin() ; it != allPortList.end(); ++it) {
        if((*it).name !=(allPortList.back()).name)
            tbFileString = tbFileString+" ." +(*it).name +"  (" + (*it).name + "),\n";
        else
            tbFileString = tbFileString+" ." +(*it).name +"  (" + (*it).name + ")\n";
    }
    tbFileString = tbFileString + ");\n";
    tbFileString = tbFileString + "\n\n";



    tbFileString = tbFileString+"endmodule\n";



    fprintf (pTBFile,tbFileString.c_str());
    fclose(pTBFile);

    return 0 ; // status OK.
}

std::vector<Clock> extractClocksList(std::string clkStr)
{
    std::vector<Clock> retClks;
    std::string clkSubStr = clkStr.substr(1);
    clkSubStr.pop_back();
    char commaKey =',';
    char *cKey = &commaKey;
    std::vector<char *> vectorClks =  splitString(clkSubStr.c_str(),cKey);
    for (std::vector<char *>::iterator it = vectorClks.begin() ; it != vectorClks.end(); ++it) {
        char colonKey =':';
        char *clKey = &colonKey;
        std::vector<char *> singleClkVector =  splitString(*it,clKey);
        if(!singleClkVector.empty()&& singleClkVector.size()==2) {
            Clock clk;
            clk.name = singleClkVector[0];
            clk.period = std::stoi( singleClkVector[1]);
            retClks.push_back(clk);
        }

    }
    return  retClks;
}


#include <cstdio>
int getTestVectors(std::string fileName,std::vector<Port> &portList) {
    if(fileName.empty())
        return 1;
    std::ifstream tvFile(fileName);

    // String to store each line of the file.
    std::string line="";

    if (tvFile.is_open()) {
        // Read each line from the file and store it in the
        // 'line' variable.
        while (std::getline(tvFile, line)) {
            printf("%s\n",line.c_str());
            char * lineT = trimWhiteSpace(const_cast<char*>(line.c_str()));
            if(line[0]=='#')
                continue;
            else {
                char * lineT = trimWhiteSpace(const_cast<char*>(line.c_str()));
                for(int i=0;i<=(int)strlen(lineT);++i) {
                    if(!portList.at(i).bus_size.empty()) {
                        int busSize = getBusSize(portList.at(i));
                        int j=0;
                        for(busSize;busSize>0;--busSize) {
                            portList.at(i).test_queue.push(lineT[j+i]);
                            ++j;
                        }
                        i = (j+i);
                    } else
                        portList.at(i).test_queue.push(lineT[i]);
                }
            }
        }

        // Close the file stream once all lines have been
        // read.
        tvFile.close();
    }
    else {
        // Print an error message to the standard error
        // stream if the file cannot be opened.
        printf("Error opening test vec file!\n") ;
        return 1;
    }
    return  0;
}

int getBusSize(Port bus) {
    std::string busRange = bus.bus_size;
    std::string busRemovedBrackets = busRange.substr(1,busRange.size()-2);
    //char ch =':';
    //char *key = &ch;
    char *key = ":";
    std::vector<char *> splitVector;
    splitVector = splitString(busRemovedBrackets.c_str(),key);
    if(splitVector.size()!=2)
        return 0;
    int leftRangeVal = std::stoi(splitVector[0]);
    int rightRangeVal = std::stoi(splitVector[1]);
    if(leftRangeVal>rightRangeVal)
        return ((leftRangeVal-rightRangeVal)+1);
    else
        return ((rightRangeVal-leftRangeVal)+1);
    return 0;
}
