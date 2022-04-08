// This is the "command" schema ("cmd") used by appfwk applications.
//
// It governs the expected top-level structure of command objects.
//
// Its objects hold substructure as an "any" type which is expected to
// be interpreted by the next lower layer such as described in the
// appfwk "application" schema ("app").

local moo = import "moo.jsonnet";
local s = moo.oschema.schema("dunedaq.appfwk.app");
local s_rccmd = import "rcif/cmd.jsonnet";
local s_iomgr = import "iomanager/connection.jsonnet";

local iomgr = moo.oschema.hier(s_iomgr).dunedaq.iomanager.connection;
local rccmd = moo.oschema.hier(s_rccmd).dunedaq.rcif.cmd;
local cmd = moo.oschema.hier(s_rccmd).dunedaq.cmdlib.cmd;

// A temporary schema construction context.
local cs = {

    plugin: s.string("PluginName", moo.re.ident_only,
                     doc="Name of a plugin"),
    inst: s.string("InstName", moo.re.ident_only,
                   doc="Name of a target instance of a kind"),
    label: s.string("Label", moo.re.ident_only,
                   doc="A label hard-wired into code"),
    
    mspec: s.record("ModSpec", [
        s.field("plugin", self.plugin,
                doc="Name of a plugin providing the module"),
        s.field("inst", self.inst,
                doc="Module instance name"),
        s.field("data", cmd.Data, optional=true,
                doc="Specific to the module implementation"),
    ], doc="Module specification"),

    mspecs: s.sequence("ModSpecs", self.mspec,
                       doc="A sequence of ModSpec"),

    modinit: s.record("ModInit", [
        s.field("cinfos", iomgr.ConnectionRefs_t,
                doc="Information for a module to find its connection"),
    ], doc="A standardized portion of every ModSpec.data"),
        
    init: s.record("Init", [
        s.field("modules", self.mspecs,
                doc="Initial Module specifications"),
        s.field("connections", iomgr.ConnectionIds_t, optional=true,
                doc="Initial connection specifications"),
    ], doc="The app-level init command data object struction"),

};

// Output a topologically sorted array.
s_iomgr + s_rccmd + moo.oschema.sort_select(cs)
