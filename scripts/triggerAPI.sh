#!/usr/bin/env bash

if [ -z $AtlasVersion ]; then
    source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh
    asetup AthenaP1,21.1,latest,slc6
else
    echo Using $AtlasVersion
fi

python -i -c "
print ('from TriggerMenu.api.TriggerAPI import TriggerAPI')
from TriggerMenu.api.TriggerAPI import TriggerAPI
print ('from TriggerMenu.api.TriggerEnums import TriggerPeriod, TriggerType')
from TriggerMenu.api.TriggerEnums import TriggerPeriod, TriggerType
"
