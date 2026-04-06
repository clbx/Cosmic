import React from 'react';
import { DockviewWillDropEvent, DockviewDidDropEvent, IWatermarkPanelProps, IDockviewHeaderActionsProps, IDockviewPanelHeaderProps, IDockviewPanelProps, DockviewOptions, DockviewReadyEvent } from 'dockview-core';
export interface IDockviewReactProps extends DockviewOptions {
    tabComponents?: Record<string, React.FunctionComponent<IDockviewPanelHeaderProps>>;
    components: Record<string, React.FunctionComponent<IDockviewPanelProps>>;
    watermarkComponent?: React.FunctionComponent<IWatermarkPanelProps>;
    defaultTabComponent?: React.FunctionComponent<IDockviewPanelHeaderProps>;
    rightHeaderActionsComponent?: React.FunctionComponent<IDockviewHeaderActionsProps>;
    leftHeaderActionsComponent?: React.FunctionComponent<IDockviewHeaderActionsProps>;
    prefixHeaderActionsComponent?: React.FunctionComponent<IDockviewHeaderActionsProps>;
    onReady: (event: DockviewReadyEvent) => void;
    onDidDrop?: (event: DockviewDidDropEvent) => void;
    onWillDrop?: (event: DockviewWillDropEvent) => void;
}
export declare const DockviewReact: React.ForwardRefExoticComponent<IDockviewReactProps & React.RefAttributes<HTMLDivElement>>;
