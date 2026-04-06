import React from 'react';
import { SplitviewApi, SplitviewPanelApi, SplitviewOptions } from 'dockview-core';
import { PanelParameters } from '../types';
export interface SplitviewReadyEvent {
    api: SplitviewApi;
}
export interface ISplitviewPanelProps<T extends {
    [index: string]: any;
} = any> extends PanelParameters<T> {
    api: SplitviewPanelApi;
    containerApi: SplitviewApi;
}
export interface ISplitviewReactProps extends SplitviewOptions {
    onReady: (event: SplitviewReadyEvent) => void;
    components: Record<string, React.FunctionComponent<ISplitviewPanelProps>>;
}
export declare const SplitviewReact: React.ForwardRefExoticComponent<ISplitviewReactProps & React.RefAttributes<HTMLDivElement>>;
