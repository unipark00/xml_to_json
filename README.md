# Simple 'XmlToJson' converter

## Overview

* xml to json by using boost::property_tree
* need xml decoration with "xmlattr"

## Xml Format (Draft)
* Give type information at attribute
```
<a type="integer">xxx</a>
<b type="string">xxx</b>
<c type="array:CCC">xxx</c>
<d type="sequence">xxx</d> --> if omitted, default "sequence"
```
* Sample
```
<nrf-pdu>
    <validityPeriod type="integer">7</validityPeriod>
    <nfInstances type="array:NFProfile">
        <NFProfile>
            <nfInstanceId type="string">3fa85f64-5717-4562-b3fc-2c963f66afa6</nfInstanceId>
            <plmnList type="array:PlmnId">
                <PlmnId>
                    <mcc type="string">45000</mcc>
                    <mnc type="string">005</mnc>
                </PlmnId>
                <PlmnId>
                    <mcc type="string">45000</mcc>
                    <mnc type="string">006</mnc>
                </PlmnId>
            </plmnList>
            <perPlmnSnssaiList type="array:PlmnSnssai">
                <PlmnSnssai>
                    <plmnId>
                        <mcc type="string">45000</mcc>
                        <mnc type="string">005</mnc>
                    </plmnId>
                    <sNssaiList type="array:Snssai">
                        <Snssai>
                            <sst type="integer">0</sst>
                            <sd type="string">1234567890</sst>
                        </Snssai>
                        <Snssai>
                            <sst type="integer">1</sst>
                            <sd type="string">abcdefghij</sst>
                        </Snssai>
                    </sNssaiList>
                </PlmnSnssai>
            </perPlmnSnssaiList>
        </NFProfile>
        <NFProfile>
            <nfInstanceId type="string">3fa85f64-5717-4562-b3fc-2c963f66afa7</nfInstanceId>
            <plmnList type="array:PlmnId">
                <PlmnId>
                    <mcc type="string">45000</mcc>
                    <mnc type="string">005</mnc>
                </PlmnId>
            </plmnList>
            <perPlmnSnssaiList type="array:PlmnSnssai">
                <PlmnSnssai>
                    <plmnId>
                        <mcc type="string">45000</mcc>
                        <mnc type="string">005</mnc>
                    </plmnId>
                    <sNssaiList type="array:Snssai">
                        <Snssai>
                            <sst type="integer">0</sst>
                            <sd type="string">1234567890</sst>
                        </Snssai>
                        <Snssai>
                            <sst type="integer">1</sst>
                            <sd type="string">abcdefghij</sst>
                        </Snssai>
                    </sNssaiList>
                </PlmnSnssai>
            </perPlmnSnssaiList>
        </NFProfile>
    </nfInstances>
    <nrfSupportedFeatures type="string">80000000</nrfSupportedFeatures>
</nrf-pdu>
```
