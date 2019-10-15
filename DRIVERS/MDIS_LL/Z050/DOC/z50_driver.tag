<?xml version='1.0' encoding='ISO-8859-1' standalone='yes'?>
<tagfile>
  <compound kind="page">
    <filename>index</filename>
    <title></title>
    <name>index</name>
  </compound>
  <compound kind="file">
    <name>d302i.h</name>
    <path>/opt/menlinux/DRIVERS/MDIS_LL/Z050/DRIVER/COM/</path>
    <filename>d302i_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>D302I_INPUT</name>
      <anchor>a0</anchor>
      <arglist>(n)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>D302I_OUTPUT</name>
      <anchor>a1</anchor>
      <arglist>(n)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>D302I_EDGE_RAISE</name>
      <anchor>a2</anchor>
      <arglist>(n)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>D302I_EDGE_FALL</name>
      <anchor>a3</anchor>
      <arglist>(n)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>D302I_IRQ_EDGE_RAISE</name>
      <anchor>a4</anchor>
      <arglist>(n)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>D302I_IRQ_EDGE_FALL</name>
      <anchor>a5</anchor>
      <arglist>(n)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>D302I_DEBOUNCE</name>
      <anchor>a6</anchor>
      <arglist>(n)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>D302I_CTRL</name>
      <anchor>a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>D302I_CTRL_GIEN</name>
      <anchor>a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>D302I_CTRL_GIP</name>
      <anchor>a9</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>z50_doc.c</name>
    <path>/opt/menlinux/DRIVERS/MDIS_LL/Z050/DRIVER/COM/</path>
    <filename>z50__doc_8c</filename>
  </compound>
  <compound kind="file">
    <name>z50_drv.c</name>
    <path>/opt/menlinux/DRIVERS/MDIS_LL/Z050/DRIVER/COM/</path>
    <filename>z50__drv_8c</filename>
    <class kind="struct">LL_HANDLE</class>
    <member kind="define">
      <type>#define</type>
      <name>_NO_LL_HANDLE</name>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CH_NUMBER</name>
      <anchor>a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>USE_IRQ</name>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ADDRSPACE_COUNT</name>
      <anchor>a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ADDRSPACE_SIZE</name>
      <anchor>a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OSH</name>
      <anchor>a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DBG_MYLEVEL</name>
      <anchor>a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DBH</name>
      <anchor>a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BIOC_INPUT</name>
      <anchor>a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BIOC_OUTPUT</name>
      <anchor>a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BIOC_EDGE_RAISE</name>
      <anchor>a10</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BIOC_EDGE_FALL</name>
      <anchor>a11</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BIOC_IRQ_EDGE_RAISE</name>
      <anchor>a12</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BIOC_IRQ_EDGE_FALL</name>
      <anchor>a13</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BIOC_CONTROL</name>
      <anchor>a14</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BIOC_DEBOUNCE</name>
      <anchor>a15</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z50_Init</name>
      <anchor>a17</anchor>
      <arglist>(DESC_SPEC *descSpec, OSS_HANDLE *osHdl, MACCESS *ma, OSS_SEM_HANDLE *devSemHdl, OSS_IRQ_HANDLE *irqHdl, LL_HANDLE **llHdlP)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z50_Exit</name>
      <anchor>a18</anchor>
      <arglist>(LL_HANDLE **llHdlP)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z50_Read</name>
      <anchor>a19</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, int32 *value)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z50_Write</name>
      <anchor>a20</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, int32 value)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z50_SetStat</name>
      <anchor>a21</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, int32 code, INT32_OR_64 value32_or_64)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z50_GetStat</name>
      <anchor>a22</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, int32 code, INT32_OR_64 *value32_or_64P)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z50_BlockRead</name>
      <anchor>a23</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, void *buf, int32 size, int32 *nbrRdBytesP)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z50_BlockWrite</name>
      <anchor>a24</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, void *buf, int32 size, int32 *nbrWrBytesP)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z50_Irq</name>
      <anchor>a25</anchor>
      <arglist>(LL_HANDLE *llHdl)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z50_Info</name>
      <anchor>a26</anchor>
      <arglist>(int32 infoType,...)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>char *</type>
      <name>Ident</name>
      <anchor>a27</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Cleanup</name>
      <anchor>a28</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 retCode)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>void</type>
      <name>gpioReset</name>
      <anchor>a29</anchor>
      <arglist>(LL_HANDLE *llHdl)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>__Z50_GetEntry</name>
      <anchor>a30</anchor>
      <arglist>(LL_ENTRY *drvP)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>const char</type>
      <name>IdentString</name>
      <anchor>a16</anchor>
      <arglist>[]</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>z50_drv.h</name>
    <path>/opt/menlinux/INCLUDE/COM/MEN/</path>
    <filename>z50__drv_8h</filename>
    <class kind="struct">Z50_DEBOUNCE_PB</class>
    <member kind="define">
      <type>#define</type>
      <name>Z50_SET_PORTS</name>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z50_CLR_PORTS</name>
      <anchor>a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z50_EDGE_RAISE</name>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z50_EDGE_FALL</name>
      <anchor>a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z50_IRQ_EDGE_RAISE</name>
      <anchor>a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z50_IRQ_EDGE_FALL</name>
      <anchor>a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z50_SET_SIGNAL</name>
      <anchor>a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z50_CLR_SIGNAL</name>
      <anchor>a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z50_BLK_DEBOUNCE</name>
      <anchor>a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z50_VARIANT</name>
      <anchor>a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_Z50_GLOBNAME</name>
      <anchor>a10</anchor>
      <arglist>(var, name)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z50_GLOBNAME</name>
      <anchor>a11</anchor>
      <arglist>(var, name)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>__Z50_GetEntry</name>
      <anchor>a12</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT32_OR_64</name>
      <anchor>a13</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>U_INT32_OR_64</name>
      <anchor>a14</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>INT32_OR_64</type>
      <name>MDIS_PATH</name>
      <anchor>a15</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>z50_simp.c</name>
    <path>/opt/menlinux/DRIVERS/MDIS_LL/Z050/EXAMPLE/Z50_SIMP/COM/</path>
    <filename>z50__simp_8c</filename>
    <member kind="define">
      <type>#define</type>
      <name>DELAY</name>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAXINP</name>
      <anchor>a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BIT_SHIFT</name>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>void</type>
      <name>PrintError</name>
      <anchor>a4</anchor>
      <arglist>(char *info)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>void __MAPILIB</type>
      <name>SignalHandler</name>
      <anchor>a5</anchor>
      <arglist>(u_int32 sig)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>char *</type>
      <name>bitString</name>
      <anchor>a6</anchor>
      <arglist>(char *s, u_int32 val, u_int32 nrBits)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int</type>
      <name>setDebounce</name>
      <anchor>a7</anchor>
      <arglist>(MDIS_PATH path, u_int32 input, u_int16 value)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int</type>
      <name>getDebounce</name>
      <anchor>a8</anchor>
      <arglist>(MDIS_PATH path, u_int32 input, u_int16 *value)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>main</name>
      <anchor>a9</anchor>
      <arglist>(int argc, char *argv[])</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>int</type>
      <name>G_sigCount</name>
      <anchor>a3</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>LL_HANDLE</name>
    <filename>structLL__HANDLE.html</filename>
    <member kind="variable">
      <type>int32</type>
      <name>memAlloc</name>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>OSS_HANDLE *</type>
      <name>osHdl</name>
      <anchor>o1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>OSS_IRQ_HANDLE *</type>
      <name>irqHdl</name>
      <anchor>o2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DESC_HANDLE *</type>
      <name>descHdl</name>
      <anchor>o3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>MACCESS</type>
      <name>ma</name>
      <anchor>o4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>MDIS_IDENT_FUNCT_TBL</type>
      <name>idFuncTbl</name>
      <anchor>o5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>u_int32</type>
      <name>dbgLevel</name>
      <anchor>o6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DBG_HANDLE *</type>
      <name>dbgHdl</name>
      <anchor>o7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>OSS_SIG_HANDLE *</type>
      <name>portChangeSig</name>
      <anchor>o8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int32</type>
      <name>d302i</name>
      <anchor>o9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>u_int32</type>
      <name>edgeRaiseIrq</name>
      <anchor>o10</anchor>
      <arglist>[CH_NUMBER]</arglist>
    </member>
    <member kind="variable">
      <type>u_int32</type>
      <name>edgeFallIrq</name>
      <anchor>o11</anchor>
      <arglist>[CH_NUMBER]</arglist>
    </member>
    <member kind="variable">
      <type>u_int32</type>
      <name>irqCount</name>
      <anchor>o12</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>u_int32</type>
      <name>inputReg</name>
      <anchor>o13</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>u_int32</type>
      <name>outputReg</name>
      <anchor>o14</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>u_int32</type>
      <name>edgeRaiseReg</name>
      <anchor>o15</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>u_int32</type>
      <name>edgeFallReg</name>
      <anchor>o16</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>u_int32</type>
      <name>irqEdgeRaiseReg</name>
      <anchor>o17</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>u_int32</type>
      <name>irqEdgeFallReg</name>
      <anchor>o18</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>u_int32</type>
      <name>debounceReg</name>
      <anchor>o19</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>u_int32</type>
      <name>controlReg</name>
      <anchor>o20</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Z50_DEBOUNCE_PB</name>
    <filename>structZ50__DEBOUNCE__PB.html</filename>
    <member kind="variable">
      <type>int</type>
      <name>input</name>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>time</name>
      <anchor>o1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="page">
    <name>dummy</name>
    <title></title>
    <filename>dummy</filename>
  </compound>
</tagfile>
