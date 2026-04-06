import React from 'react';
import { PaneviewPanelApi, PaneviewApi, PaneviewDropEvent, PaneviewOptions } from 'dockview-core';
import { PanelParameters } from '../types';
export interface PaneviewReadyEvent {
    api: PaneviewApi;
}
export interface IPaneviewPanelProps<T extends {
    [index: string]: any;
} = any> extends PanelParameters<T> {
    api: PaneviewPanelApi;
    containerApi: PaneviewApi;
    title: string;
}
export interface IPaneviewReactProps extends PaneviewOptions {
    onReady: (event: PaneviewReadyEvent) => void;
    components: Record<string, React.FunctionComponent<IPaneviewPanelProps>>;
    headerComponents?: Record<string, React.FunctionComponent<IPaneviewPanelProps>>;
    onDidDrop?(event: PaneviewDropEvent): void;
}
export declare const PaneviewReact: React.ForwardRefExoticComponent<IPaneviewReactProps & React.RefAttributes<HTMLDivElement>>;
