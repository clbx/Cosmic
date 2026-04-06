/// <reference types="react" />
import { GridviewPanel, IFrameworkPart } from 'dockview-core';
import { ReactPortalStore } from '../react';
import { IGridviewPanelProps } from './gridview';
export declare class ReactGridPanelView extends GridviewPanel {
    private readonly reactComponent;
    private readonly reactPortalStore;
    constructor(id: string, component: string, reactComponent: React.FunctionComponent<IGridviewPanelProps>, reactPortalStore: ReactPortalStore);
    getComponent(): IFrameworkPart;
}
