# UpdatingScriptInCPP
college thesis work: 

goal is to make an updating script using cmd commands like winget with gui



implemented:

reading and loading txt file data

breaking lines into only words

get and set symbol encoding on cmd (needed because windows makes txt file in ansi encoding)

find process by name and get its process id

terminate process by given process id

checks if user has launched script with administrator

checks if internet exist

check if msstore source is available in winget

checks which programmes need updating

interactable gui items like list and buttons fully implement

displaing which programmes to update and allowing user to select programme they want to update using list and then button to confirm their request

updating programmes


discarded idea:

--killing guis launched by programmes being updating (to automate, this programme is later updated in manual mode) (will not be implemented because killing gui means winget packet manager can no longer access uninstaller as programmes use same id (this include the custom version filter (that part doesn't work on every programme and starts deleting latest one)))
